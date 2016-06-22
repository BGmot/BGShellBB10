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

    lblFontSample = new QLabel(tr("Sample 123"), this);
    lblFontSample->setObjectName(tr("lblFontSample"));
    QFont fontSample = QFont(QString("Courier New"), 6);
    fontSample.setPixelSize(mainWindow->nFontSize);
    fontSample.setStyle(QFont::StyleNormal);
    fontSample.setWeight(QFont::Normal);
    lblFontSample->setFont(fontSample);

    this->setObjectName(tr("wdgSettingsWindow"));
    QComboBox *cbFontSize = new QComboBox(gb1);
    cbFontSize->setObjectName(tr("cbFontSize"));
    cbFontSize->setMaxVisibleItems(9);
    for(int i=18; i < 52; i += 4){
      cbFontSize->addItem(QString::number(i));
      if (i == mainWindow->nFontSize)
        cbFontSize->setCurrentIndex(cbFontSize->count()-1); // Current Font size
    }

    hlayout->addWidget(lblFontSample);
    hlayout->addWidget(cbFontSize);

    layout->addWidget(gb1);

    btnClose = new QToolButton(this);
    btnClose->setObjectName(QString::fromUtf8("btnClose"));
    btnClose->setText(QString("Close"));
    btnClose->setMinimumWidth(nWidth/2);
    layout->addWidget(btnClose);
    layout->setAlignment(btnClose, Qt::AlignHCenter);

    gb->setLayout(layout);

    QMetaObject::connectSlotsByName(this);

	return 0;
}

extern QTermWidget *console;
void CMySettingsWindow::on_cbFontSize_currentIndexChanged(const QString & text){
    // User selected something from drop-down list
    int nSize = text.toInt();
	mainWindow->nFontSize = nSize;

	// Save new settings
	QSettings settings;
	settings.setValue("FontSize", nSize);

	// Create new Font
	QFont newfont = QFont(QString("Courier New"), 6);
	newfont.setPixelSize(nSize);
    newfont.setStyle(QFont::StyleNormal);
    newfont.setWeight(QFont::Normal);

    // Apply new Font/size to Sample Text
    lblFontSample->setFont(newfont);

    // Apply new Font/size to console
    console->setTerminalFont(newfont);
}

void CMySettingsWindow::on_btnClose_clicked(){
    QEvent* event= new QEvent(QEvent::PlatformPanel);
    QApplication::sendEvent(this->parent(), event);
}
