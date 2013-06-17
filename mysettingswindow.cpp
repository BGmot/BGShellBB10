/*
 * Copyright (C) 2013 BGmot <support@tm-k.com>
 */
#include <QtGui/QPalette>
#include <QtGui/QLabel>
#include <QtCore/QSettings>
#include "qtermwidget.h"
#include "mymainwindow.h"
#include "mysettingswindow.h"

extern CMyMainWindow *mainWindow;

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

    // HTTP Proxy block
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(20, 10, 20, 20);

    QLabel *lblProxy = new QLabel(tr("You might need to use HTTP Proxy for SSH connections if you installed the app in Work perimeter or you are behind restrictive firewall"
    		" but have HTTP proxy allowing CONNECT method and this proxy's IP/port configured in your smartphone's Settings"));
    lblProxy->setWordWrap(true);
    lblProxy->setAlignment(Qt::AlignHCenter);
    layout->addWidget(lblProxy);

    cbxProxyOn = new QCheckBox(tr(" Use HTTP Proxy"));
	QSettings settings;
	int nProxyOn = settings.value("HttpProxyOn").toInt();
	if (nProxyOn)
		cbxProxyOn->setChecked(true);
	else
		cbxProxyOn->setChecked(false);
    layout->addWidget(cbxProxyOn);

    // Font size block
    QGroupBox *gb1 = new QGroupBox(tr("Terminal Font Size"), this);
    gb1->setAlignment(Qt::AlignHCenter);
    gb1->setFlat(false);
    QHBoxLayout *hlayout = new QHBoxLayout;
    gb1->setLayout(hlayout);
    QRadioButton *rbtnSmall	 = new QRadioButton(tr("Small"));
    QRadioButton *rbtnNorm	 = new QRadioButton(tr("Norm"));
    QRadioButton *rbtnLarge	 = new QRadioButton(tr("Large"));
    QRadioButton *rbtnXLarge = new QRadioButton(tr("XLarge"));
    switch (mainWindow->nFontSize){
    case 20:
    	rbtnSmall->setChecked(true);
    	break;
    case 22:
    	rbtnNorm->setChecked(true);
    	break;
    case 24:
    	rbtnLarge->setChecked(true);
    	break;
    case 26:
    	rbtnXLarge->setChecked(true);
    	break;
    default:
    	rbtnNorm->setChecked(true);
    	break;
    }
    btngrFontSize = new QButtonGroup();
    btngrFontSize->addButton(rbtnSmall);
    btngrFontSize->addButton(rbtnNorm);
    btngrFontSize->addButton(rbtnLarge);
    btngrFontSize->addButton(rbtnXLarge);
    QObject::connect(btngrFontSize, SIGNAL(buttonPressed(QAbstractButton *)), this, SLOT(on_FontSizeChanged(QAbstractButton *)));
    hlayout->addWidget(rbtnSmall);
    hlayout->addWidget(rbtnNorm);
    hlayout->addWidget(rbtnLarge);
    hlayout->addWidget(rbtnXLarge);
    layout->addWidget(gb1);

    QLabel *lblHint = new QLabel(tr("\nHint: Swipe down again to hide this dialog"));
    layout->addWidget(lblHint);

    gb->setLayout(layout);

	return 0;
}

extern QTermWidget *console;
void CMySettingsWindow::on_FontSizeChanged(QAbstractButton *button){
	QString btn = button->text();
	int nSize = 22;
	if (!btn.compare(QString("Small"))){
		nSize = 20;
	}else if (!btn.compare(QString("Norm"))){
		nSize = 22;
	}else if (!btn.compare(QString("Large"))){
		nSize = 24;
	}else if (!btn.compare(QString("XLarge"))){
		nSize = 26;
	}
	mainWindow->nFontSize = nSize;
	QSettings settings;
	settings.setValue("FontSize", nSize);

	QFont newfont;
	newfont.setPixelSize(nSize);
    newfont.setStyle(QFont::StyleNormal);
    newfont.setWeight(QFont::Normal);
	console->setTerminalFont(newfont);
}
