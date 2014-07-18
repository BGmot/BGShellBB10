/*
 * Copyright (C) 2013 BGmot <support@tm-k.com>
 */
#ifndef MYMAINWINDOW_H
#define MYMAINWINDOW_H
#include <QtGui/QMainWindow>
#include <bb/platform/PaymentManager>

class CMyMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	bb::platform::PaymentManager *paymentManager;
	int nFontSize;

    CMyMainWindow();
    virtual bool event(QEvent *e);
    void ReadAndApplyProxySettings();

public Q_SLOTS:
    //This method is called whenever a purchase is invoked
    void purchaseResponse();
};

#endif
