/*
 * Copyright (C) 2013 BGmot <support@tm-k.com>
 */
#ifndef MYMAINWINDOW_H
#define MYMAINWINDOW_H
#include <QtGui/QMainwindow>

class CMyMainWindow : public QMainWindow
{
	Q_OBJECT

public:
    CMyMainWindow();
    virtual bool event(QEvent *e);
};

#endif
