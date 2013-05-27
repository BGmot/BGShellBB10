/*
 * Copyright (C) 2013 BGmot <support@tm-k.com>
 */
#ifndef MYDONATESWINDOW_H
#define MYDONATESWINDOW_H
#include <QtGui/QWidget>
#include <QtGui/QRadioButton>
#include <QtGui/QToolButton>

class CMyDonateWindow : public QWidget
{
	Q_OBJECT

public:
    CMyDonateWindow(QWidget *parent);
    int Init();

    QRadioButton *rbtnOne;
    QRadioButton *rbtnTwo;
    QRadioButton *rbtnThree;
    QRadioButton *rbtnFive;
    QRadioButton *rbtnTen;

    QToolButton *btnOk;
    QToolButton *btnCancel;

private slots:
	void on_btnOk_clicked();
	void on_btnCancel_clicked();
};

#endif
