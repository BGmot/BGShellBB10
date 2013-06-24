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
#include <QtGui/QApplication>
#include <fcntl.h>
#include "qtermwidget.h"
#include "mymenu.h"
#include "mysystemmenu.h"
#include "mymainwindow.h"
#ifndef BBQ10
#include "bps/virtualkeyboard.h"
#include "bps/bps.h"
#endif
#ifdef BBQ10
#include "myvk.h"
#endif

int masterFdG = -1; // will be used in parent process
int slaveFdG = -1;  // will be used in child process
int pidG_ = -1;     // child's PID
#ifndef BBQ10
int nKBHeight = 0;  // current Virtual Keyboard height
int nMaxKBHeight = 0; // Maximum Keyboard Height in current mode (Landscape/Portrait)
#endif
QTermWidget *console; // our 'main' widget, let's make it global so it is available in Menu widget
CMyMainWindow *mainWindow;
CMyMenu *Menu;      // Menu with soft buttons
CMySystemMenu *SystemMenu; // System menu from 'swipe down' event
QFont font;
#ifndef BBQ10
bool bOrientationJustChanged = false; // If we just changed screen orientation then do not handle hide/show keyboard event (skip one ScreenAvailableGeometry event)
bool bKBhidden = false; // True if VK is not shown
#endif
bool bCtrlFlag = false; // A flag that we use to interpret key - whether it should be treated as Ctrl+ or not
#ifdef BBQ10
CMyVirtualKeyboard *virtualKeyboard;
bool bSymFlag = false; // A flag that we use to check whether VK is shown
#endif

#ifndef BBQ10
static QAbstractEventDispatcher::EventFilter mainEventFilter = 0; // To store old EventFilter (Application's)

bool myEventFilter(void *message) {
	// here we should have all requested bps events (no Qt events here)
    bps_event_t *event = (bps_event_t*)message;
    if (event) {
    	int domain = bps_event_get_domain(event);
    	if (domain == virtualkeyboard_get_domain()) {
    		unsigned int ec = bps_event_get_code(event);
    		 if (!bKBhidden && ec == VIRTUALKEYBOARD_EVENT_INFO){
    			int nNewKBHeight = virtualkeyboard_event_get_height(event);
    			virtualkeyboard_get_height(&nNewKBHeight);
    			if ( nNewKBHeight != nKBHeight){
    				nKBHeight = nNewKBHeight;
    				QRect r = QApplication::desktop()->screenGeometry(0);
    				if (r.width() > 800){
    					// Landscape
    					console->setGeometry(0, 0, r.width()-73, r.height()-nKBHeight);
    				}else{
    					// Portrait - keyboard is taller
    					console->setGeometry(0, 53, r.width(), r.height()-nKBHeight-53);
    				}
    			}
    		}
    		QRect r = QApplication::desktop()->screenGeometry(0);
    		if (ec == VIRTUALKEYBOARD_EVENT_VISIBLE){
				bKBhidden = false;
				nKBHeight = nMaxKBHeight;
				virtualkeyboard_change_options(VIRTUALKEYBOARD_LAYOUT_DEFAULT, VIRTUALKEYBOARD_ENTER_DEFAULT);
    		}else if (ec == VIRTUALKEYBOARD_EVENT_HIDDEN){
				bKBhidden = true;
				nKBHeight = 0;
    		}
			if (r.width() > 800) console->setGeometry(0, 0, r.width()-73, r.height()-nKBHeight); // Landscape
			else console->setGeometry(0, 103, r.width(), r.height()-nKBHeight-103); // Portrait

			//font = QFont(QString("Courier New"), 6);
			font.setPixelSize(mainWindow->nFontSize);
		    font.setStyle(QFont::StyleNormal);
		    font.setWeight(QFont::Normal);
			console->setTerminalFont(font);
    	}
   	}
 	mainEventFilter(message); // Call replaced event filter so we deliever everything to Qt that runs in background
	return false;
}
#endif

int main(int argc, char *argv[])
{
    // Let's search for programs first in our own 'app' folder
    QString oldPath = qgetenv("PATH");
    QString newPath = "app/native:"+oldPath;
    qputenv("PATH", newPath.toAscii().data());

    // We need to store our working folder somewhere, it is needed for ssh later
    int res = system ("echo -n `pwd`\"/data/\" > data/.myhome");
    if (res == -1){
    	fprintf(stderr, "Can't determine my home folder location, terminating");
    	exit(-1);
    }

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
        const char *arglist[] = {"/bin/sh", "-l", NULL};
        execvp(arglist[0], (char**)arglist);
    } else if (pidG_ == -1) {
        // fork() failed
  	    qDebug() << "fork() failed:";
        pidG_ = 0;
        return false;
    }
    // Drop garbage that is shown when you start the app
    char cGarbage[72];
    read(masterFdG, cGarbage, 72);

    // needed for correct QT initialization
    //qputenv("QT_QPA_FONTDIR", "/usr/fonts/font_repository/dejavu-ttf-2.17"); // needed for correct QT initialization

    qputenv("QT_QPA_PLATFORM_PLUGIN_PATH", "/usr/lib/qt4/plugins/platforms");

    //QCoreApplication::addLibraryPath("app/native/lib"); // blackberry plugin does not load without this line

    QApplication app(argc, argv);

    mainWindow = new CMyMainWindow();
    QRect r = QApplication::desktop()->screenGeometry(0);
    mainWindow->resize(r.width()+1, r.height()+1);

    console = new QTermWidget(1, mainWindow);

    font = QFont(QString("Courier New"), 6);
    font.setPixelSize(mainWindow->nFontSize);
    font.setStyle(QFont::StyleNormal);
    font.setWeight(QFont::Normal);
	console->setTerminalFont(font);

#ifndef BBQ10
    // We start the app with shown keyboard
	virtualkeyboard_change_options(VIRTUALKEYBOARD_LAYOUT_DEFAULT, VIRTUALKEYBOARD_ENTER_DEFAULT);
	if (r.width() > 800){
		// Landscape
		virtualkeyboard_get_height(&nKBHeight);
		console->setGeometry(0, 0, r.width()-73, r.height()-nKBHeight);
	}else{
		// Portrait - keyboard is higher
		virtualkeyboard_get_height(&nKBHeight);
		console->setGeometry(0, 103, r.width()+1, r.height()-nKBHeight-102);
	}
#else
	console->setGeometry(0, 0, r.width()+1, r.height()-103);
#endif
    console->setScrollBarPosition(QTermWidget::ScrollBarRight);

    // Our 'soft buttons' menu
    Menu = new CMyMenu(mainWindow);
    Menu->MenuInit();

    // Our system menu
    SystemMenu = new CMySystemMenu(mainWindow);
    SystemMenu->MenuInit();

    QObject::connect(console, SIGNAL(finished()), mainWindow, SLOT(close()));

    mainWindow->show();    

#ifndef BBQ10
    // Install event filter
    mainEventFilter = QAbstractEventDispatcher::instance()->setEventFilter(myEventFilter);

    // Show virtual keyboard
    QEvent event(QEvent::RequestSoftwareInputPanel);
    QApplication::sendEvent(console, &event);
    virtualkeyboard_get_height(&nMaxKBHeight); // Init maximum VK height

    virtualkeyboard_request_events(0); // To catch show/hide VK events
#endif
#ifdef BBQ10
    virtualKeyboard = new CMyVirtualKeyboard(mainWindow);
#endif
    return app.exec();
}
