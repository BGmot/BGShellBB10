/*
 * Copyright (C) 2012 BGmot <support@tm-k.com>
 */
#ifndef MYVK_H
#define MYVK_H

#include <QtGui/QLabel>

class CMyVirtualKeyboard : public QLabel
{
	Q_OBJECT

public:
    CMyVirtualKeyboard(QWidget *parent);
    virtual ~CMyVirtualKeyboard(){};
};
#endif
