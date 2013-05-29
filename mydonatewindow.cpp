/*
 * Copyright (C) 2013 BGmot <support@tm-k.com>
 */
#include <QtGui/QPalette>
#include <QtGui/QLabel>
#include <QtGui/QRadioButton>
#include <qplatformnativeinterface_qpa.h>
#include <bps/paymentservice.h>
#include "qtermwidget.h"
#include "mydonatewindow.h"
//#include "mymainwindow.h"
#include "mysystemmenu.h"
#include "qtermwidget.h"

//extern CMyMainWindow *mainWindow;
extern CMySystemMenu *SystemMenu;
extern QTermWidget *console;

CMyDonateWindow::CMyDonateWindow(QWidget *parent) :
	QWidget(parent)
{
	setWindowModality(Qt::ApplicationModal);
}

int CMyDonateWindow::Init(){
	QRect r = QApplication::desktop()->screenGeometry(0);
	int nWidth = r.width();
	int nHeight = nWidth;
	this->setGeometry(0, 0, nWidth, nHeight);

    QBrush brush1(QColor(76, 76, 76, 255));
    brush1.setStyle(Qt::SolidPattern);
    QPalette palette;
    palette.setBrush(QPalette::Active, QPalette::Window, brush1);
    this->setAutoFillBackground(true);

    QVBoxLayout *layoutGlobal = new QVBoxLayout;
    this->setLayout(layoutGlobal);

    QGroupBox *gb = new QGroupBox(tr("Your Thanks is appreciated"));
    gb->setGeometry(10, 2, this->width()-20, this->height()-20);

    gb->setAlignment(Qt::AlignHCenter);
    gb->setFlat(false);
    layoutGlobal->addWidget(gb);

    QVBoxLayout *layoutV1 = new QVBoxLayout;
    layoutV1->setContentsMargins(20, 10, 20, 20);
    gb->setLayout(layoutV1);

    QLabel *lblWelcome = new QLabel(tr("Receiving simple \"Thank you\" is always encouraging and inspiring. There are many ways to say thanks: "
    		"just an e-mail at bgmot@tm-k.com or if you feel like ready to spend several bucks here is your chance to say thanks this way:\n\n"));
    lblWelcome->setWordWrap(true);
    lblWelcome->setAlignment(Qt::AlignHCenter);
    lblWelcome->setContentsMargins(10, 5, 10, 5);
    layoutV1->addWidget(lblWelcome);

    QWidget *wdgRadioBtns = new QWidget();
    QVBoxLayout *layoutV2 = new QVBoxLayout;
    layoutV2->setContentsMargins(200, 0, 20, 0);
    wdgRadioBtns->setLayout(layoutV2);

    rbtnOne	= new QRadioButton(tr("$1"));
    rbtnOne->setChecked(true);
    rbtnTwo 	= new QRadioButton(tr("$2"));
    rbtnThree	= new QRadioButton(tr("$3"));
    rbtnFive	= new QRadioButton(tr("$5"));
    rbtnTen	= new QRadioButton(tr("$10 (are you sure?)"));
    layoutV2->addWidget(rbtnOne);
    layoutV2->addWidget(rbtnTwo);
    layoutV2->addWidget(rbtnThree);
    layoutV2->addWidget(rbtnFive);
    layoutV2->addWidget(rbtnTen);

    layoutV1->addWidget(wdgRadioBtns);


    QWidget *wdgButtons = new QWidget();
    QHBoxLayout *layoutH = new QHBoxLayout;
    layoutH->setContentsMargins(20, 10, 20, 20);
    wdgButtons->setLayout(layoutH);
    layoutGlobal->addWidget(wdgButtons);

	btnOk = new QToolButton(this);
    btnOk->setObjectName(QString::fromUtf8("btnOk"));
    btnOk->setText(QString("Thanks!"));
    btnOk->setMinimumWidth(nWidth/3);
    layoutH->addWidget(btnOk);

	btnCancel = new QToolButton(this);
    btnCancel->setObjectName(QString::fromUtf8("btnCancel"));
    btnCancel->setText(QString("Next time"));
    btnCancel->setMinimumWidth(nWidth/3);
    layoutH->addWidget(btnCancel);

    QMetaObject::connectSlotsByName(this);

    return 0;
}
void CMyDonateWindow::on_btnOk_clicked(){
	SystemMenu->hide();
	close();

	// Find Window Group ID
	QPlatformNativeInterface * const nativeInterface = QApplication::platformNativeInterface();
	//const char * const groupName = static_cast<const char *>(nativeInterface->nativeResourceForWidget("windowGroup", QApplication::topLevelWidgets().first()));
	//const char * const groupName = static_cast<const char *>(nativeInterface->nativeResourceForWidget("windowGroup", mainWindow));
	const char * const groupName = static_cast<const char *>(nativeInterface->nativeResourceForWidget("windowGroup", console));

    const char* purchase_app_name = "BG SSH-SCP-SFTP";
    unsigned request_id = 0; // Define a request ID to hold the returned value from the purchase request
    //const char* metadata = "";
    //const char* purchase_app_icon = "http://www.rim.com/products/appworld_3col.jpg";

    if (rbtnOne->isChecked()){
	    //const char* digital_good_id = "One-ID";
	    const char* digital_good_name = "ThanksOne";
	    const char* digital_good_sku = "SKU_BG-SSH-SCP-SFTP_1";
	    // Initiate the application with a purchase of the sample digital good.
	    if (paymentservice_purchase_request(NULL/*digital_good_id*/, digital_good_sku, digital_good_name,
	            NULL/*metadata*/, purchase_app_name, NULL/*purchase_app_icon*/, groupName/*get_window_group_id()*/, &request_id) != BPS_SUCCESS) {
	    	qDebug() << QString("CMyDonateWindow::on_btnOk_clicked(): paymentservice_purchase_request() failed when purchasing ThanksOne");
	    }
	}else if (rbtnTwo->isChecked()){
		const char* digital_good_name = "ThanksTwo";
		const char* digital_good_sku = "SKU_BG-SSH-SCP-SFTP_2";
		// Initiate the application with a purchase of the sample digital good.
		if (paymentservice_purchase_request(NULL/*digital_good_id*/, digital_good_sku, digital_good_name,
				NULL/*metadata*/, purchase_app_name, NULL/*purchase_app_icon*/, groupName/*get_window_group_id()*/, &request_id) != BPS_SUCCESS) {
	    	qDebug() << QString("CMyDonateWindow::on_btnOk_clicked(): paymentservice_purchase_request() failed when purchasing ThanksTwo");
		}
	}else if (rbtnThree->isChecked()){
		const char* digital_good_name = "ThanksThree";
		const char* digital_good_sku = "SKU_BG-SSH-SCP-SFTP_3";
		// Initiate the application with a purchase of the sample digital good.
		if (paymentservice_purchase_request(NULL/*digital_good_id*/, digital_good_sku, digital_good_name,
				NULL/*metadata*/, purchase_app_name, NULL/*purchase_app_icon*/, groupName/*get_window_group_id()*/, &request_id) != BPS_SUCCESS) {
	    	qDebug() << QString("CMyDonateWindow::on_btnOk_clicked(): paymentservice_purchase_request() failed when purchasing ThanksThree");
		}
	}else if (rbtnFive->isChecked()){
		const char* digital_good_name = "ThanksFive";
		const char* digital_good_sku = "SKU_BG-SSH-SCP-SFTP_5";
		// Initiate the application with a purchase of the sample digital good.
		if (paymentservice_purchase_request(NULL/*digital_good_id*/, digital_good_sku, digital_good_name,
				NULL/*metadata*/, purchase_app_name, NULL/*purchase_app_icon*/, groupName/*get_window_group_id()*/, &request_id) != BPS_SUCCESS) {
	    	qDebug() << QString("CMyDonateWindow::on_btnOk_clicked(): paymentservice_purchase_request() failed when purchasing ThanksFive");
		}
	}else if (rbtnTen->isChecked()){
		const char* digital_good_name = "ThanksTen";
		const char* digital_good_sku = "SKU_BG-SSH-SCP-SFTP_10";
		// Initiate the application with a purchase of the sample digital good.
		if (paymentservice_purchase_request(NULL/*digital_good_id*/, digital_good_sku, digital_good_name,
				NULL/*metadata*/, purchase_app_name, NULL/*purchase_app_icon*/, groupName/*get_window_group_id()*/, &request_id) != BPS_SUCCESS) {
	    	qDebug() << QString("CMyDonateWindow::on_btnOk_clicked(): paymentservice_purchase_request() failed when purchasing ThanksTwo");
		}
	}

	close();
}
void CMyDonateWindow::on_btnCancel_clicked(){
	SystemMenu->hide();
	close();
}
