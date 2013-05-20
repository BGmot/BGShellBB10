/*
 * Copyright (C) 2013 BGmot <support@tm-k.com>
 */

#include "mysystemmenu.h"
#include "qtermwidget.h"

CMySystemMenu::CMySystemMenu(QWidget *parent) :
QWidget(parent)
{
	setVisible(false);
	setWindowModality(Qt::ApplicationModal);
}

// There is nothing to comment here, all button names tell you what they do
int CMySystemMenu::MenuInit(){
	btnSettings = new QToolButton(this);
	btnSettings->setObjectName(QString::fromUtf8("btnSettings"));
	btnSettings->setText(QString("Settings"));

	btnDonate = new QToolButton(this);
	btnDonate->setObjectName(QString::fromUtf8("btnDonate"));
	btnDonate->setText(QString("Donate"));

    QMetaObject::connectSlotsByName(this);

    SetGeometry();

	return 0;
}
void CMySystemMenu::SetGeometry(){
	QRect r = QApplication::desktop()->screenGeometry(0);
	this->setGeometry(0, 0, r.width(), 103);
	btnSettings->setGeometry(QRect(2, 1, 310, 101));
	btnDonate  ->setGeometry(QRect(312, 1, 310, 101));
}

void CMySystemMenu::on_btnSettings_clicked(){
	return;
}
void CMySystemMenu::on_btnDonate_clicked(){
	return;
}
