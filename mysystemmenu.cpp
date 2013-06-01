/*
 * Copyright (C) 2013 BGmot <support@tm-k.com>
 */
#include "qtermwidget.h"
#include "mysystemmenu.h"
#include "mydonatewindow.h"
#include "mysettingswindow.h"

CMySystemMenu::CMySystemMenu(QWidget *parent) :
QWidget(parent)
{
	wdgSettingsWindow = NULL;
	wdgDonateWindow = NULL;
	setVisible(false);
	//setWindowModality(Qt::ApplicationModal);
}

// There is nothing to comment here, all button names tell you what they do
int CMySystemMenu::MenuInit(){
	btnSettings = new QToolButton(this);
	btnSettings->setObjectName(QString::fromUtf8("btnSettings"));
	btnSettings->setText(QString("Settings"));

	btnDonate = new QToolButton(this);
	btnDonate->setObjectName(QString::fromUtf8("btnDonate"));
	btnDonate->setText(QString("Say Thanks"));

	// For now disable donations
	btnDonate->setVisible(false);

    QMetaObject::connectSlotsByName(this);

    SetGeometry();

	return 0;
}
void CMySystemMenu::SetGeometry(){
	QRect r = QApplication::desktop()->screenGeometry(0);
	int nScreenWidth = r.width();
	this->setGeometry(0, 0, nScreenWidth, 103);
	//this->setMinimumWidth(nScreenWidth);
    QBrush brush1(QColor(76, 76, 76, 255));
    brush1.setStyle(Qt::SolidPattern);
    QPalette palette;
    palette.setBrush(QPalette::Active, QPalette::Window, brush1);
    this->setAutoFillBackground(true);

	btnSettings->setGeometry(QRect(2, 1, nScreenWidth/2-2, 101));
	btnDonate  ->setGeometry(QRect(nScreenWidth/2+2, 1, nScreenWidth/2-4, 101));
}

void CMySystemMenu::on_btnSettings_clicked(){
	if (!wdgSettingsWindow){
		wdgSettingsWindow = new CMySettingsWindow((QWidget*)this->parent());
		wdgSettingsWindow->Init();
		wdgSettingsWindow->show();
	}
	return;
}
void CMySystemMenu::on_btnDonate_clicked(){
	if (!wdgDonateWindow){
		wdgDonateWindow = new CMyDonateWindow((QWidget*)this->parent());
		wdgDonateWindow->Init();
	}
	wdgDonateWindow->show();
	return;
}
