/*
 * Copyright (C) 2013 BGmot <support@tm-k.com>
 */
#include <QtCore/QDebug>
#include <QtCore/QEvent>
#include <QtCore/QSettings>
#include <QtCore/QFile>
#include <bps/netstatus.h>

#include "mymainwindow.h"
#include "mysystemmenu.h"
#include "mysettingswindow.h"

extern CMySystemMenu *SystemMenu;
#define SSH_CONFIG_FNAME "data/.ssh/config"
#define SSH_CONFIG_FNAME_TMP "data/.ssh/config.tmp"
#define SSH_PROXY_AUTH_FNAME "data/.ssh/myauth"

CMyMainWindow::CMyMainWindow() :
	QMainWindow()
{
	ReadAndApplyProxySettings();
}

bool CMyMainWindow::event(QEvent *e){
//	qDebug()<<"CMyMainWindow::event() type="<<e->type();
	if (e->type() ==  QEvent::PlatformPanel){
		// Up-down swipe detected
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

	QSettings settings;
	int nProxyOn = settings.value("HttpProxyOn").toInt();
	if (nProxyOn &&!host.isEmpty()){
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
			QString proxycommand = "\tProxyCommand corkscrew " + host + " " + port + " %h %p\n";
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
