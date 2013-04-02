/*
 * Copyright (C) 2012 BGmot <support@tm-k.com>
 */

#include <fcntl.h>
#include "mymenu.h"
#include "qtermwidget.h"

extern int masterFdG;
extern QTermWidget *console;
extern bool bCtrlFlag;

CMyMenu::CMyMenu(QWidget *parent) :
QWidget(parent)
{
}

// There is nothing to comment here, all button names tell you what they do
int CMyMenu::MenuInit(){
	btnCtrlC = new QToolButton(this);
    btnCtrlC->setObjectName(QString::fromUtf8("btnCtrlC"));
    btnCtrlC->setText(QString("Ctrl+"));

	btnTab = new QToolButton(this);
    btnTab->setObjectName(QString::fromUtf8("btnTab"));
    btnTab->setText(QString("Tab"));

    btnLeft = new QToolButton(this);
    btnLeft->setObjectName(QString::fromUtf8("btnLeft"));
    btnLeft->setText(QString("<"));

    btnRight = new QToolButton(this);
    btnRight->setObjectName(QString::fromUtf8("btnRight"));
    btnRight->setText(QString(">"));

    btnUp = new QToolButton(this);
    btnUp->setObjectName(QString::fromUtf8("btnUp"));
    btnUp->setText(QString("^"));

    btnDown = new QToolButton(this);
    btnDown->setObjectName(QString::fromUtf8("btnDown"));
    btnDown->setText(QString("v"));

    btnEsc = new QToolButton(this);
    btnEsc->setObjectName(QString::fromUtf8("btnEsc"));
    btnEsc->setText(QString("Esc"));

    QMetaObject::connectSlotsByName(this);
	QRect r = QApplication::desktop()->screenGeometry(0); // Landscape 1023x599
	if (r.width() > 800)
		SetGeometryLandscape();
	else
		SetGeometryPortrait();

	return 0;
}
void CMyMenu::SetGeometryPortrait(){
	QRect r = QApplication::desktop()->screenGeometry(0);
	this->setGeometry(0, 0, r.width(), 103);
	btnCtrlC->setGeometry(QRect(2, 1, 110, 101));
	btnTab  ->setGeometry(QRect(112, 1, 100, 101));
	btnLeft ->setGeometry(QRect(112+1*100+1*2, 1, 100, 101));
	btnRight->setGeometry(QRect(112+2*100+2*2, 1, 100, 101));
	btnUp   ->setGeometry(QRect(112+3*100+3*2, 1, 100, 101));
	btnDown ->setGeometry(QRect(112+4*100+4*2, 1, 100, 101));
	btnEsc  ->setGeometry(QRect(112+5*100+5*2, 1, 100, 101));
}
void CMyMenu::SetGeometryLandscape(){
	this->setGeometry(1206, 0, 73, 357);
    btnCtrlC->setGeometry(QRect(1, 1, 71, 51));
    btnTab->setGeometry(QRect(1, 52, 71, 51));
    btnLeft->setGeometry(QRect(1, 103, 71, 51));
    btnRight->setGeometry(QRect(1, 154, 71, 51));
    btnUp->setGeometry(QRect(1, 205, 71, 51));
    btnDown->setGeometry(QRect(1, 256, 71, 51));
    btnEsc->setGeometry(QRect(1, 307, 71, 50));
}

void CMyMenu::on_btnCtrlC_clicked(){
	// Now it is not Ctrl+C, it is CTRL+something, so let's wait for the next key or reset this flag
	if (bCtrlFlag == false) bCtrlFlag = true;
	else bCtrlFlag = false;
	console->setFocus();
	return;
}
void CMyMenu::on_btnTab_clicked(){
	char c = 9;
	write(masterFdG, &c, 1);
	console->setFocus();
	return;
}
void CMyMenu::on_btnLeft_clicked(){
	char c[] = {0x1B,0x5B,'D'};
	write(masterFdG, c, 3);
	console->setFocus();
	return;
}
void CMyMenu::on_btnRight_clicked(){
	char c[] = {0x1B,0x5B,'C'};
	write(masterFdG, c, 3);
	console->setFocus();
	return;
}
void CMyMenu::on_btnUp_clicked(){
	char c[] = {0x1B,0x5B,'A'};
	write(masterFdG, c, 3);
	console->setFocus();
	return;
}
void CMyMenu::on_btnDown_clicked(){
	char c[] = {0x1B,0x5B,'B'};
	write(masterFdG, c, 3);
	console->setFocus();
	return;
}
void CMyMenu::on_btnEsc_clicked(){
	char c = 0x1B;
	write(masterFdG, &c, 1);
	console->setFocus();
	return;
}
