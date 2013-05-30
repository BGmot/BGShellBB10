/*
 * Copyright (C) 2013 BGmot <support@tm-k.com>
 */
#include <QtGui/QPalette>
#include <QtGui/QLabel>
#include <QtGui/QRadioButton>
#include <qplatformnativeinterface_qpa.h>
#include <bps/paymentservice.h>
#include <bb/platform/PurchaseReply>
#include "qtermwidget.h"
#include "mydonatewindow.h"
#include "mysystemmenu.h"

#include "mymainwindow.h"

extern CMyMainWindow *mainWindow;
extern CMySystemMenu *SystemMenu;

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

	QString sku, name;

	if (rbtnOne->isChecked()){
	    sku = QString("SKU_BG-SSH-SCP-SFTP_1");
	    name = QString("ThanksOne");
    }else if (rbtnTwo->isChecked()){
	    sku = QString("SKU_BG-SSH-SCP-SFTP_2");
	    name = QString("ThanksTwo");
	}else if (rbtnThree->isChecked()){
	    sku = QString("SKU_BG-SSH-SCP-SFTP_3");
	    name = QString("ThanksThree");
	}else if (rbtnFive->isChecked()){
	    sku = QString("SKU_BG-SSH-SCP-SFTP_5");
	    name = QString("ThanksFive");
	}else if (rbtnTen->isChecked()){
	    sku = QString("SKU_BG-SSH-SCP-SFTP_10");
	    name = QString("ThanksTen");
	}
	// Initiate purchase
    const bb::platform::PurchaseReply *reply = mainWindow->paymentManager->requestPurchase(NULL, sku, name, NULL);
    connect(reply, SIGNAL(finished()), mainWindow, SLOT(purchaseResponse()));
}
void CMyDonateWindow::on_btnCancel_clicked(){
	SystemMenu->hide();
	close();
}
