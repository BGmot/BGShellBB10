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
	//this->setGeometry(950, 0, 73, 355);
	this->setGeometry(950, 0, 73, 357);

	btnCtrlC = new QPushButton(this);
    btnCtrlC->setGeometry(QRect(1, 1, 71, 51));
    btnCtrlC->setObjectName(QString::fromUtf8("btnCtrlC"));
    btnCtrlC->setText(QString("Ctrl+C"));

	btnTab = new QPushButton(this);
    btnTab->setGeometry(QRect(1, 52, 71, 51));
    btnTab->setObjectName(QString::fromUtf8("btnTab"));
    btnTab->setText(QString("Tab"));

    btnLeft = new QPushButton(this);
    btnLeft->setGeometry(QRect(1, 103, 71, 51));
    btnLeft->setObjectName(QString::fromUtf8("btnLeft"));
    btnLeft->setText(QString("<"));

    btnRight = new QPushButton(this);
    btnRight->setGeometry(QRect(1, 154, 71, 51));
    btnRight->setObjectName(QString::fromUtf8("btnRight"));
    btnRight->setText(QString(">"));

    btnUp = new QPushButton(this);
    btnUp->setGeometry(QRect(1, 205, 71, 51));
    btnUp->setObjectName(QString::fromUtf8("btnUp"));
    btnUp->setText(QString("^"));

    btnDown = new QPushButton(this);
    btnDown->setGeometry(QRect(1, 256, 71, 51));
    btnDown->setObjectName(QString::fromUtf8("btnDown"));
    btnDown->setText(QString("v"));

    btnEsc = new QPushButton(this);
    btnEsc->setGeometry(QRect(1, 307, 71, 50));
    btnEsc->setObjectName(QString::fromUtf8("btnEsc"));
    btnEsc->setText(QString("Esc"));

    QMetaObject::connectSlotsByName(this);

	return 0;
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
