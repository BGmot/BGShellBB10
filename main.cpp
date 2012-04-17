/*  Copyright (C) 2008 e_k (e_k@users.sourceforge.net)

    Ported to Blackberry Playbook by BGmot <support@tm-k.com>, 2012

    This application is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.
		
    This application is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.
				
    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/
						

#include <QtCore>
#include <QtGui>
#include <QApplication>
#include <QtGui/private/qwindowsysteminterface_qpa_p.h>
#include <fcntl.h>
#include "qtermwidget.h"
#include "mymenu.h"
//#include "qbbvirtualkeyboard.h"
//#include <QDebug>


int masterFdG = -1; // will be used in parent process
int slaveFdG = -1;  // will be used in child process
int pidG_ = -1;     // child's PID
int nKBHeight = 0;  // Virtual Keyboard height
QTermWidget *console; // our 'main' widget, let's make it global so it is available in Menu widget
QMainWindow *mainWindow;
CMyMenu *Menu;      // Menu with soft buttons
bool bOrientationJustChanged = false; // If we just changed screen orientation then do not handle hide/show keyboard event (skip one ScreenAvailableGeometry event)

bool myEventFilter(void *message) {
	// I don't know the other way to detect user's 'show/hide virtual keyboard', so let's catch all events
	QWindowSystemInterfacePrivate::WindowSystemEvent *msg;
	msg = (QWindowSystemInterfacePrivate::WindowSystemEvent*)message;
	//qDebug() << "+++event type:" << msg->type;
	if (msg->type == QWindowSystemInterfacePrivate::ScreenAvailableGeometry){
		// VK has been hidden/showed, let's update stored VKB height and change available console's space
		//qDebug()<<"myeventfilter msg->type = ScreenAvailableGeometry";
		//nKBHeight = QBBVirtualKeyboard::instance().getHeight(); - nasty hack, does not work at Simulator -(( Don't know other way
		if (!bOrientationJustChanged){
			QRect r = QApplication::desktop()->screenGeometry(0);
			if (r.width() > 700){
				nKBHeight == 0 ? nKBHeight = 244 : nKBHeight = 0; // Landscape
				console->setGeometry(0, 0, r.width()-73, r.height()-nKBHeight);
			}else{
				nKBHeight == 0 ? nKBHeight = 264 : nKBHeight = 0; // Portrait - keyboard is taller
				console->setGeometry(0, 53, r.width(), r.height()-nKBHeight-53);
			}
		}else
			bOrientationJustChanged = false;
	}else if (msg->type == QWindowSystemInterfacePrivate::ScreenGeometry){
		// Orientation changed
		QRect r = QApplication::desktop()->screenGeometry(0); // Landscape 1023x599
		mainWindow->resize(r.width()+1, r.height()+1);

		if (r.width() > 700){
			// Landscape
			if (nKBHeight > 0) nKBHeight = 244; // only if VK is currently shown - VK is lower when in Landscape
			console->setGeometry(0, 0, r.width()-73, r.height()-nKBHeight);
			Menu->SetGeometryLandscape();
		}else{
			// Portrait
			if (nKBHeight > 0) nKBHeight = 264; // only if VK is currently shown - VK is taller when in Portrait
			console->setGeometry(0, 53, r.width(), r.height()-nKBHeight-53);
			Menu->SetGeometryPortrait();
		}
		bOrientationJustChanged = true;

		// For some reason font is screwed when orientation changes, let's fix 'constant width'
		QFont font = console->font();
		font.setStyleStrategy(QFont::ForceIntegerMetrics); // we must do it to get normal cursor functionality
	    console->setTerminalFont(font);
	}

	return false;
}

int main(int argc, char *argv[])
{
    // Let's search for programs first in our own 'app' folder
    QString oldPath = qgetenv("PATH");
    QString newPath = "app/native:"+oldPath;
    qputenv("PATH", newPath.toAscii().data());

	// Init file descriptors opening proper devices
	masterFdG = ::open("/dev/ptyp1", O_RDWR);
	slaveFdG = ::open("/dev/ttyp1", O_RDWR | O_NOCTTY);
	// And here is where magic begins
    pidG_ = fork();
    if (pidG_ == 0) {
        dup2(slaveFdG, STDIN_FILENO);
        dup2(slaveFdG, STDOUT_FILENO);
        dup2(slaveFdG, STDERR_FILENO);

        // reset all signal handlers
        struct sigaction act;
        sigemptyset(&act.sa_mask);
        act.sa_handler = SIG_DFL;
        act.sa_flags = 0;
        for (int sig = 1; sig < NSIG; sig++)
          sigaction(sig, &act, 0L);

        // All we want in child process is to have sh running
        const char *arglist[] = {"/bin/sh", NULL};
        execvp(arglist[0], (char**)arglist);
    } else if (pidG_ == -1) {
        // fork() failed
  	    qDebug() << "fork() failed:";
        pidG_ = 0;
        return false;
    }
    // Drop garbage that is shown when you start the app
    char cGarbage[63];
    read(masterFdG, cGarbage, 63);

    qputenv("QT_QPA_FONTDIR", "/usr/fonts/font_repository/monotype"); // needed for correct QT initialization

    QCoreApplication::addLibraryPath("app/native/lib"); // blackberry plugin does not load without this line

    QApplication app(argc, argv);

    mainWindow = new QMainWindow();
    QRect r = QApplication::desktop()->screenGeometry(0); // Landscape 1023x599
    mainWindow->resize(r.width()+1, r.height()+1);

    console = new QTermWidget(1, mainWindow);

    // We start the app with shown keyboard
	if (r.width() > 700){
		nKBHeight = 244; // Landscape
		console->setGeometry(0, 0, r.width()-73, r.height()-nKBHeight);
	}else{
		nKBHeight = 264; // Portrait - keyboard is higher
		console->setGeometry(0, 53, r.width()+1, r.height()-nKBHeight-52);
	}

	QFont font = QApplication::font();
    //font.setFamily("Terminus");
    font.setPointSize(14);
    font.setStyleStrategy(QFont::ForceIntegerMetrics); // we must do it to get normal cursor functionality
    console->setTerminalFont(font);

    console->setScrollBarPosition(QTermWidget::ScrollBarRight);

    // Our 'soft buttons' menu
    Menu = new CMyMenu(mainWindow);
    Menu->MenuInit();

    QObject::connect(console, SIGNAL(finished()), mainWindow, SLOT(close()));

    mainWindow->show();    

    // Install event filter
    QAbstractEventDispatcher *aed = QAbstractEventDispatcher::instance(app.thread());
    aed->setEventFilter(myEventFilter);

    // Show virtual keyboard
    QEvent event(QEvent::RequestSoftwareInputPanel);
    QApplication::sendEvent(console, &event);

    return app.exec();
}
