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

    SetGeometryPortrait();

	return 0;
}

void CMyMenu::SetGeometryPortrait(){
	QRect r = QApplication::desktop()->screenGeometry(0);
	int nW = r.width();
	int btnW = nW/7;
	this->setGeometry(0, 0, nW, 103);
	btnCtrlC->setGeometry(QRect(2, 1, btnW-1, 101));
	btnTab  ->setGeometry(QRect(1*btnW+1, 1, btnW-1, 101));
	btnLeft ->setGeometry(QRect(2*btnW+1, 1, btnW-1, 101));
	btnRight->setGeometry(QRect(3*btnW+1, 1, btnW-1, 101));
	btnUp   ->setGeometry(QRect(4*btnW+1, 1, btnW-1, 101));
	btnDown ->setGeometry(QRect(5*btnW+1, 1, btnW-1, 101));
	btnEsc  ->setGeometry(QRect(6*btnW+1, 1, btnW-1, 101));
}

void CMyMenu::on_btnCtrlC_clicked(){
	// Now it is not Ctrl+C, it is CTRL+something, so let's wait for the next key or reset this flag
	if (bCtrlFlag == false){
		bCtrlFlag = true;
		btnCtrlC->setDown(true);
	}
	else{
		bCtrlFlag = false;
		btnCtrlC->setDown(false);
	}
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
    if(bCtrlFlag)
         c[2] = 'd';
    write(masterFdG, c, 3);
    console->setFocus();
    return;
}
void CMyMenu::on_btnRight_clicked(){
    char c[] = {0x1B,0x5B,'C'};
    if(bCtrlFlag)
         c[2] = 'c';
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
