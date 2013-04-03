/*
 * Copyright (C) 2012 BGmot <support@tm-k.com>
 */
#ifndef MYMENU_H
#define MYMENU_H
#include <QtGui/QWidget>
//#include <QtGui/QPushButton>
#include <QtGui/QToolButton>

class CMyMenu : public QWidget
{
	Q_OBJECT

public:
    CMyMenu(QWidget *parent);
    int MenuInit();
    void SetGeometryPortrait();
    void SetGeometryLandscape();
    virtual ~CMyMenu(){};
	//void focusOutEvent( QFocusEvent * event );

    QToolButton *btnCtrlC;
    QToolButton *btnTab;
    QToolButton *btnLeft;
    QToolButton *btnRight;
    QToolButton *btnUp;
    QToolButton *btnDown;
    QToolButton *btnEsc;
private slots:
	void on_btnCtrlC_clicked();
	void on_btnTab_clicked();
	void on_btnLeft_clicked();
	void on_btnRight_clicked();
	void on_btnUp_clicked();
	void on_btnDown_clicked();
	void on_btnEsc_clicked();
};

#endif
