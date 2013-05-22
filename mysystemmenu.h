/*
 * Copyright (C) 2013 BGmot <support@tm-k.com>
 */
#ifndef MYSYSTEMMENU_H
#define MYSYSTEMMENU_H
#include <QtGui/QWidget>
#include <QtGui/QToolButton>
#include "mysettingswindow.h"

class CMySystemMenu : public QWidget
{
	Q_OBJECT

public:
    CMySystemMenu(QWidget *parent);
    int MenuInit();
    void SetGeometry();
    virtual ~CMySystemMenu(){};

    QToolButton *btnSettings;
    QToolButton *btnDonate;
    CMySettingsWindow *wdgSettingsWindow;

private slots:
	void on_btnSettings_clicked();
	void on_btnDonate_clicked();
};

#endif
