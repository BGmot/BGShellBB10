/*
 * Copyright (C) 2013 BGmot <support@tm-k.com>
 */
#ifndef MYSETTINGSWINDOW_H
#define MYSETTINGSWINDOW_H
#include <QtGui/QWidget>
#include <QtGui/QCheckBox>
#include <QtGui/QLabel>
#include <QtGui/QToolButton>

class CMySettingsWindow : public QWidget
{
	Q_OBJECT

public:
    CMySettingsWindow(QWidget *parent);
    int Init();

    QCheckBox *cbxProxyOn;
    QLabel *lblFontSample;
    QToolButton *btnClose;
    
private slots:
    void on_cbFontSize_currentIndexChanged(const QString &);
    void on_btnClose_clicked();
};

#endif
