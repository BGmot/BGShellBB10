/*
 * Copyright (C) 2012 BGmot <support@tm-k.com>
 */
#ifdef BBQ10
#ifndef MYVK_H
#define MYVK_H

#include <QtGui/QLabel>

class CMyVirtualKeyboard : public QLabel
{
	Q_OBJECT

public:
    CMyVirtualKeyboard(QWidget *parent);
    //void SetGeometry();
    virtual ~CMyVirtualKeyboard(){};

//private slots:
//	void on_btnCtrlC_clicked();
};
#endif
#endif
