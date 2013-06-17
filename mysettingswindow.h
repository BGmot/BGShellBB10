/*
 * Copyright (C) 2013 BGmot <support@tm-k.com>
 */
#ifndef MYSETTINGSWINDOW_H
#define MYSETTINGSWINDOW_H
#include <QtGui/QWidget>
#include <QtGui/QCheckBox>
#include <QtGui/QButtonGroup>

class CMySettingsWindow : public QWidget
{
	Q_OBJECT

public:
    CMySettingsWindow(QWidget *parent);
    int Init();

    QCheckBox *cbxProxyOn;
    QButtonGroup *btngrFontSize;
private slots:
	void on_FontSizeChanged(QAbstractButton *);
};

#endif
