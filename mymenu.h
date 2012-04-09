/*
 * Copyright (C) 2012 BGmot <support@tm-k.com>
 */
#ifndef MYMENU_H
#define MYMENU_H
#include <QWidget>
#include <QPushButton>

class CMyMenu : public QWidget
{
	Q_OBJECT

public:
    CMyMenu(QWidget *parent);
    int MenuInit();
	//void focusOutEvent( QFocusEvent * event );
private:
    QPushButton *btnCtrlC;
    QPushButton *btnTab;
    QPushButton *btnLeft;
    QPushButton *btnRight;
    QPushButton *btnUp;
    QPushButton *btnDown;
    QPushButton *btnEsc;
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
