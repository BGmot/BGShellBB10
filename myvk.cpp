/*
 * Copyright (C) 2013 BGmot <support@tm-k.com>
 */
#include <QtGui/QPixmap>
#include "qtermwidget.h"
#include "myvk.h"

CMyVirtualKeyboard::CMyVirtualKeyboard(QWidget *parent)
	: QLabel(parent)
{
	setVisible(false);
	QRect r = QApplication::desktop()->screenGeometry(0);
	QPixmap image("app/native/assets/vk.png");
	QDir dir;
	setPixmap(image);
	setGeometry(0, r.height()-image.height(), image.width(), image.height());
}
