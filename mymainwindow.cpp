/*
 * Copyright (C) 2013 BGmot <support@tm-k.com>
 */
#include <QtCore/QDebug>
#include <QtCore/QEvent>
#include "mymainwindow.h"
#include "mysystemmenu.h"

extern CMySystemMenu *SystemMenu;

CMyMainWindow::CMyMainWindow() :
	QMainWindow()
{
}

bool CMyMainWindow::event(QEvent *e){
	qDebug()<<"CMyMainWindow::event() type="<<e->type();
	if (e->type() ==  QEvent::PlatformPanel){
		// Up-down swipe detected
		if (SystemMenu->isVisible() == false)
			SystemMenu->show();
		else
			SystemMenu->hide();
	}

	return QMainWindow::event(e);
}
