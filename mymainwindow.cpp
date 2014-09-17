/*
 * Copyright (C) 2013 BGmot <support@tm-k.com>
 */
#include <QtCore/QDebug>
#include <QtCore/QEvent>
#include <QtCore/QSettings>
#include <QtCore/QFile>
#include <QtGui/QMessageBox>
#include <QtCore/QDir>
#include <bps/netstatus.h>
#include <qplatformnativeinterface_qpa.h>
#include <qapplication.h>
#include <bb/platform/PurchaseReply>

#include "mymainwindow.h"
#include "mysystemmenu.h"
#include "mysettingswindow.h"

extern CMySystemMenu *SystemMenu;
extern CMyMainWindow *mainWindow;
#define SSH_CONFIG_FNAME "data/.ssh/config"
#define SSH_CONFIG_FNAME_TMP "data/.ssh/config.tmp"
#define SSH_PROXY_AUTH_FNAME "data/.ssh/myauth"

CMyMainWindow::CMyMainWindow() :
	QMainWindow()
{
	QSettings settings;
	int nConfigFontSize = settings.value("FontSize").toInt();
	if (nConfigFontSize != 0)
		nFontSize = nConfigFontSize;
	else
		nFontSize = 22;
	ReadAndApplyProxySettings();
	bb::platform::PaymentManager::setConnectionMode(bb::platform::PaymentConnectionMode::Production);
	paymentManager = new bb::platform::PaymentManager(this);
    QPlatformNativeInterface * const nativeInterface = QApplication::platformNativeInterface();
    //const QString windowGroupId = static_cast<const char *>(nativeInterface->nativeResourceForWidget("windowGroup", QApplication::topLevelWidgets().first()));
    const QString windowGroupId = static_cast<const char *>(nativeInterface->nativeResourceForWidget("windowGroup", mainWindow));
    paymentManager->setWindowGroupId(windowGroupId);
}

bool CMyMainWindow::event(QEvent *e){
//	qDebug()<<"CMyMainWindow::event() type="<<e->type();
	if (e->type() ==  QEvent::PlatformPanel){
		// Swipe-down detected
		if (SystemMenu->wdgDonateWindow)
			if (SystemMenu->wdgDonateWindow->isVisible())
				return QMainWindow::event(e);

		// We react to up-swipe event only if Donate window is not visible
		if (SystemMenu->wdgSettingsWindow){
			// Swipe when Settings window is active
			// Save Proxy Settings
			QSettings settings;
			if (SystemMenu->wdgSettingsWindow->cbxProxyOn->isChecked())
				settings.setValue("HttpProxyOn", 1);
			else
				settings.setValue("HttpProxyOn", 0);
			ReadAndApplyProxySettings();

			SystemMenu->wdgSettingsWindow->close();
			delete SystemMenu->wdgSettingsWindow;
			SystemMenu->wdgSettingsWindow = NULL;
		}
		if (SystemMenu->isVisible()){
			SystemMenu->hide();
		}else{
			SystemMenu->show();
		}
	}
	return QMainWindow::event(e);
}

void CMyMainWindow::ReadAndApplyProxySettings(){
	// Read current proxy settings
    netstatus_proxy_details_t details;
    memset(&details, 0, sizeof(details));
    if (netstatus_get_proxy_details(&details) == BPS_FAILURE) {
    	qDebug() << QString("CMyMainWindow::ReadAndApplyProxySettings(): netstatus_get_proxy_details() failed");
        return;
    }
    QString host = details.http_proxy_host;
    QString port = QString::number(details.http_proxy_port);
    QString user = details.http_proxy_login_user;
    QString password = details.http_proxy_login_password;
    netstatus_free_proxy_details(&details);

    QDir SshDir(QString("data"));
    QDir SshDir1(QString("data/.ssh"));
    if (!SshDir1.exists())
    	if (!SshDir.mkdir(".ssh"))
    		qDebug() << "CMyMainWindow::ReadAndApplyProxySettings(): Can't create data/.ssh";

	QSettings settings;
	int nProxyOn = settings.value("HttpProxyOn").toInt();
	if (nProxyOn && !host.isEmpty()){
		// Proxy should be turned on

	    QString proxycommand;
		if (user.isEmpty()){
			proxycommand = "\tProxyCommand corkscrew " + host + " " + port + " %h %p\n";
			QFile fileSshProxyAuth(SSH_PROXY_AUTH_FNAME);
			if (fileSshProxyAuth.exists())
				fileSshProxyAuth.remove();
		}else{
		    // Create auth file
			QFile fileSshProxyAuth(SSH_PROXY_AUTH_FNAME);
		    if (fileSshProxyAuth.open(QIODevice::WriteOnly)){
		    	QString line = user + ":" + password + "\n";
		    	fileSshProxyAuth.write(line.toAscii());
		    	fileSshProxyAuth.close();
		    	fileSshProxyAuth.setPermissions(QFile::ReadOwner | QFile::WriteOwner);
		    	proxycommand = "\tProxyCommand corkscrew " + host + " " + port + " %h %p ~/.ssh/myauth\n";
		    }else{
				qDebug() << QString("CMyMainWindow::ReadAndApplyProxySettings(): can't open ") + QString(SSH_PROXY_AUTH_FNAME) + QString(" for writing");
				proxycommand = "\tProxyCommand corkscrew " + host + " " + port + " %h %p\n";
		    }
		}
		QFile fileSshConfig(SSH_CONFIG_FNAME);
		if (fileSshConfig.exists()){
			// file does exist
			if (!fileSshConfig.open(QIODevice::ReadOnly)){
				qDebug() << QString("CMyMainWindow::ReadAndApplyProxySettings(): can't open ") + QString(SSH_CONFIG_FNAME) + QString(" for reading");
				return;
			}
			QFile fileSshConfigTmp(SSH_CONFIG_FNAME_TMP);
			if (!fileSshConfigTmp.open(QIODevice::WriteOnly)){
				qDebug() << QString("CMyMainWindow::ReadAndApplyProxySettings(): can't open ") + QString(SSH_CONFIG_FNAME_TMP) + QString(" for writing");
				fileSshConfig.close();
				return;
			}
			bool bHostAllFound = false;
			bool bProxyCommandFound = false;
			QTextStream in(&fileSshConfig);
		    while (!in.atEnd()) {
		    	QString line = in.readLine();
		    	if (bHostAllFound){
		    		// We already saw line "Host *", searching for "ProxyCommand"
		    		if (!bProxyCommandFound){
		    			if (line.contains(QString("ProxyCommand"), Qt::CaseInsensitive)){
		    				bProxyCommandFound = true;
		    				fileSshConfigTmp.write(proxycommand.toAscii());
		    			}else{
		    				if (line.contains(QString("Host"), Qt::CaseInsensitive)){
		    					// Next "Host" block we need to write our proxy before
			    				fileSshConfigTmp.write(proxycommand.toAscii());
			    				bProxyCommandFound = true;
		    				}
		    				line += '\n';
		    				fileSshConfigTmp.write(line.toAscii());
		    			}
		    		}else{
		    			line += '\n';
		    			fileSshConfigTmp.write(line.toAscii());
		    		}
		    	}else{
		    		// "Host *" is not found yet
			    	if (line.contains(QString("Host *"), Qt::CaseInsensitive))
			    		bHostAllFound = true;
		    		line += '\n';
		    		fileSshConfigTmp.write(line.toAscii());
		    	}
		    }
	    	if (!bHostAllFound){
				fileSshConfigTmp.write("Host *\n");
				fileSshConfigTmp.write(proxycommand.toAscii());
	    	}else{
	    		if (!bProxyCommandFound)
					fileSshConfigTmp.write(proxycommand.toAscii());
	    	}
		    fileSshConfig.close();
		    fileSshConfig.remove();
		    fileSshConfigTmp.close();
		    fileSshConfigTmp.setPermissions(QFile::ReadOwner | QFile::WriteOwner);
		    fileSshConfigTmp.rename(SSH_CONFIG_FNAME);

		    return;
		}else{
			// file does not exist, let's create it
			if (!fileSshConfig.open(QIODevice::WriteOnly)){
				qDebug() << QString("CMyMainWindow::ReadAndApplyProxySettings(): can't open ") + QString(SSH_CONFIG_FNAME) + QString(" for writing");
				return;
			}
			fileSshConfig.write("Host *\n");
			fileSshConfig.write(proxycommand.toAscii());
			fileSshConfig.close();
			fileSshConfig.setPermissions(QFile::ReadOwner | QFile::WriteOwner);
		}
	}else{
		// Proxy should be turned off
		QFile fileSshProxyAuth(SSH_PROXY_AUTH_FNAME);
		if (fileSshProxyAuth.exists())
			fileSshProxyAuth.remove();

		QFile fileSshConfig(SSH_CONFIG_FNAME);
		if (fileSshConfig.exists()){
			// file does exist, let's make sure that there is no proxy for Host *
			if (!fileSshConfig.open(QIODevice::ReadOnly)){
				qDebug() << QString("CMyMainWindow::ReadAndApplyProxySettings(): can't open ") + QString(SSH_CONFIG_FNAME) + QString(" for reading");
				return;
			}
			QFile fileSshConfigTmp(SSH_CONFIG_FNAME_TMP);
			if (!fileSshConfigTmp.open(QIODevice::WriteOnly)){
				qDebug() << QString("CMyMainWindow::ReadAndApplyProxySettings(): can't open ") + QString(SSH_CONFIG_FNAME_TMP) + QString(" for writing");
				fileSshConfig.close();
				return;
			}
			QTextStream in(&fileSshConfig);
		    while (!in.atEnd()) {
		    	QString line = in.readLine();
		    	if (!line.contains(QString("ProxyCommand"), Qt::CaseInsensitive)){
		    		// Write everything except ProxyCommand into new file
		    		line += '\n';
		    		fileSshConfigTmp.write(line.toAscii());
		    	}
		    }
		    fileSshConfig.close();
		    fileSshConfig.remove();
		    fileSshConfigTmp.close();
		    fileSshConfigTmp.setPermissions(QFile::ReadOwner | QFile::WriteOwner);
		    fileSshConfigTmp.rename(SSH_CONFIG_FNAME);

		    return;
		}else{
			// file does not exist, so nothing to do
			return;

		}
	}
}

void CMyMainWindow::purchaseResponse(){
    bb::platform::PurchaseReply *reply = qobject_cast<bb::platform::PurchaseReply*>(sender());
    Q_ASSERT(reply);
    if (reply->isError()){
    	qDebug() << "CMyMainWindow::purchaseResponse(): Purchase response error. Code(" << reply->errorCode() << ") Text(" << reply->errorText() << ")";
    	QMessageBox msgBox;
    	msgBox.setText(QString("An error occured:\nCode(") + QString::number(reply->errorCode()) + QString(") Text(") + reply->errorText() + QString(")"));
    	msgBox.setStandardButtons(QMessageBox::Close);
    	msgBox.exec();
    }else{
    	bb::platform::PurchaseReceipt r = reply->receipt();
        const QString initialPeriod = QString::number(r.initialPeriod());
        const QDateTime startDate = r.startDate();
        const QString startDateStr = startDate.isNull() ? "N/A" : startDate.toString();
        const QDateTime endDate = r.endDate();
        const QString endDateStr = endDate.isNull() ? "N/A" : endDate.toString();
        const QString isSubscr = r.isSubscription() ? "true" : "false";
        const QString itemStateStr = QString::number(static_cast<int>(r.state()));

        const QString displayString = "Date: " + r.date().toString() +
            "\nID/SKU: " + r.digitalGoodId() + "/" + r.digitalGoodSku() +
            "\nPurchaseID/licenseKey: " + r.purchaseId() + "/" + r.licenseKey() +
            "\nMetadata: " + r.purchaseMetadata() +
            "\nItemState/isSubscription?: " + itemStateStr + "/" + isSubscr +
            "\nStart/End: " + startDateStr + "/" + endDateStr +
            "\nInitialPeriod: " + initialPeriod + "\n";
    	qDebug() << "Purchase response success. " << displayString;

    	 QMessageBox msgBox;
    	 msgBox.setText("Thank you!");
    	 msgBox.setStandardButtons(QMessageBox::Close);
    	 msgBox.exec();
    }
}
