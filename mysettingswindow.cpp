/*
 * Copyright (C) 2013 BGmot <support@tm-k.com>
 */
#include <QtGui/QPalette>
#include <QtGui/QLabel>
#include "qtermwidget.h"
#include "mysettingswindow.h"

CMySettingsWindow::CMySettingsWindow(QWidget *parent) :
	QWidget(parent)
{
	setWindowModality(Qt::ApplicationModal);

}

int CMySettingsWindow::Init(){
	QRect r = QApplication::desktop()->screenGeometry(0);
	int nWidth = r.width();
	int nHeight = nWidth;
	this->setGeometry(0, 0, nWidth, nHeight);

    QBrush brush1(QColor(76, 76, 76, 255));
    brush1.setStyle(Qt::SolidPattern);
    QPalette palette;
    palette.setBrush(QPalette::Active, QPalette::Window, brush1);
    this->setAutoFillBackground(true);

    QGroupBox *gb = new QGroupBox(tr("Settings"), this);
    gb->setGeometry(10, 2, this->width()-20, this->height()-20);

    gb->setAlignment(Qt::AlignHCenter);
    gb->setFlat(false);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(20, 10, 20, 20);

    QLabel *lblProxy = new QLabel(tr("You might need to use HTTP Proxy for SSH connections if you installed the app in Work perimeter or you are behind restrictive firewall"
    		" but have HTTP proxy allowing CONNECT method and this proxy's IP/port configured in your smartphone's Settings\n\n"));
    lblProxy->setWordWrap(true);
    layout->addWidget(lblProxy);

    cbxProxyOn = new QCheckBox(tr(" Use HTTP Proxy"));
	QSettings settings;
	int nProxyOn = settings.value("HttpProxyOn").toInt();
	if (nProxyOn)
		cbxProxyOn->setChecked(true);
	else
		cbxProxyOn->setChecked(false);
    layout->addWidget(cbxProxyOn);

    QLabel *lblHint = new QLabel(tr("\nHint: Swipe again to hide this dialog"));
    layout->addWidget(lblHint);

    gb->setLayout(layout);

	return 0;
}
