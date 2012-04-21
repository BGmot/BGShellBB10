/*
 * Copyright (C) 2012 BGmot <support@tm-k.com>
 */

#include <fcntl.h>
#include <QDebug>
#include "mymenu.h"
#include "qtermwidget.h"

extern int masterFdG;
extern QTermWidget *console;

CMyMenu::CMyMenu(QWidget *parent) :
QWidget(parent)
{
}

// There is nothing to comment here, all button names tell you what they do
int CMyMenu::MenuInit(){
	btnCtrlC = new QPushButton(this);
    btnCtrlC->setObjectName(QString::fromUtf8("btnCtrlC"));
    btnCtrlC->setText(QString("Ctrl+C"));

	btnTab = new QPushButton(this);
    btnTab->setObjectName(QString::fromUtf8("btnTab"));
    btnTab->setText(QString("Tab"));

    btnLeft = new QPushButton(this);
    btnLeft->setObjectName(QString::fromUtf8("btnLeft"));
    btnLeft->setText(QString("<"));

    btnRight = new QPushButton(this);
    btnRight->setObjectName(QString::fromUtf8("btnRight"));
    btnRight->setText(QString(">"));

    btnUp = new QPushButton(this);
    btnUp->setObjectName(QString::fromUtf8("btnUp"));
    btnUp->setText(QString("^"));

    btnDown = new QPushButton(this);
    btnDown->setObjectName(QString::fromUtf8("btnDown"));
    btnDown->setText(QString("v"));

    btnEsc = new QPushButton(this);
    btnEsc->setObjectName(QString::fromUtf8("btnEsc"));
    btnEsc->setText(QString("Esc"));

    QMetaObject::connectSlotsByName(this);
	QRect r = QApplication::desktop()->screenGeometry(0); // Landscape 1023x599
	if (r.width() > 700)
		SetGeometryLandscape();
	else
		SetGeometryPortrait();

	return 0;
}
void CMyMenu::SetGeometryPortrait(){
	/*this->setGeometry(537, 0, 63, 357);
    btnCtrlC->setGeometry(QRect(1, 1, 61, 51));
    btnTab->setGeometry(QRect(1, 52, 61, 51));
    btnLeft->setGeometry(QRect(1, 103, 61, 51));
    btnRight->setGeometry(QRect(1, 154, 61, 51));
    btnUp->setGeometry(QRect(1, 205, 71, 61));
    btnDown->setGeometry(QRect(1, 256, 61, 51));
    btnEsc->setGeometry(QRect(1, 307, 61, 50));
    */
	QRect r = QApplication::desktop()->screenGeometry(0);
	this->setGeometry(0, 0, r.width(), 53);
	btnCtrlC->setGeometry(QRect(2, 1, 95, 51));
	btnTab->setGeometry(QRect(97, 1, 82, 51));
	btnLeft->setGeometry(QRect(181, 1, 82, 51));
	btnRight->setGeometry(QRect(265, 1, 82, 51));
	btnUp->setGeometry(QRect(349, 1, 82, 51));
	btnDown->setGeometry(QRect(433, 1, 82, 51));
	btnEsc->setGeometry(QRect(517, 1, 82, 51));
}
void CMyMenu::SetGeometryLandscape(){
	this->setGeometry(950, 0, 73, 357);
    btnCtrlC->setGeometry(QRect(1, 1, 71, 51));
    btnTab->setGeometry(QRect(1, 52, 71, 51));
    btnLeft->setGeometry(QRect(1, 103, 71, 51));
    btnRight->setGeometry(QRect(1, 154, 71, 51));
    btnUp->setGeometry(QRect(1, 205, 71, 51));
    btnDown->setGeometry(QRect(1, 256, 71, 51));
    btnEsc->setGeometry(QRect(1, 307, 71, 50));
}

void CMyMenu::on_btnCtrlC_clicked(){
	char c = 3;
	write(masterFdG, &c, 1);
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
