/*
 * Copyright (C) 2013 BGmot <support@tm-k.com>
 */
#include <QtCore/QDebug>
#include <QtCore/QEvent>
#include "mymainwindow.h"
#include "mysystemmenu.h"
#include "mysettingswindow.h"

extern CMySystemMenu *SystemMenu;

CMyMainWindow::CMyMainWindow() :
	QMainWindow()
{
}

bool CMyMainWindow::event(QEvent *e){
	qDebug()<<"CMyMainWindow::event() type="<<e->type();
	if (e->type() ==  QEvent::PlatformPanel){
		// Up-down swipe detected
		if (SystemMenu->wdgSettingsWindow){
			// Swipe when Settings window is active
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
