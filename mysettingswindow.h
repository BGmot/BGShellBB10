/*
 * Copyright (C) 2013 BGmot <support@tm-k.com>
 */
#ifndef MYSETTINGSWINDOW_H
#define MYSETTINGSWINDOW_H
#include <QtGui/QWidget>
#include <QtGui/QCheckBox>

class CMySettingsWindow : public QWidget
{
	Q_OBJECT

public:
    CMySettingsWindow(QWidget *parent);
    int Init();

    QCheckBox *cbxProxyOn;
};

#endif
