#include <QWindow>
#include <QFileInfo>
#include <QDebug>
#include <QFile>

/// C libs
#include <cstdlib>

/// local headers
#include "windowinfo_p.hpp"

WindowInfoPrivate::WindowInfoPrivate(QObject *parent) 
	: QObject(parent) {

	/// All supported default program signs.
	m_ProgramSigns << "konsole"
		       << "gnome-terminal"
	       	       << "io.elementary.terminal";	       
}

WindowInfoPrivate::~WindowInfoPrivate() {
	quit();
	XDOWrapper::xdo_free(ctx);
}

void WindowInfoPrivate::setProgram(const QString &program) {
	m_ProgramSigns.clear();
	m_ProgramSigns << program;
}

void WindowInfoPrivate::setDebug(bool value) {
	bDebug = value;
}

void WindowInfoPrivate::start() {
	ctx = XDOWrapper::xdo_new(NULL);
	if(!ctx) {
		qDebug() << "Cannot get xdo context.";
		return;
	}

	m_Timer = new QTimer(this);
	m_Timer->setSingleShot(false);
	m_Timer->setInterval(200);

	connect(m_Timer, &QTimer::timeout,
		 this, &WindowInfoPrivate::loop);

	m_Timer->start();
}

void WindowInfoPrivate::quit() {
	if(!m_Timer) {
		return;
	}
	if(!m_Timer->isActive()) {
		return;
	}

	m_Timer->stop();
}

void WindowInfoPrivate::loop() {
	Window wid = 0;
	int r = XDOWrapper::xdo_get_focused_window_sane(ctx, &wid);
	if(r) {
		return;
	}
	if(bDebug) {
		qDebug() << "WindowInfo::loop():: " 
			 << "Window in Focus:: "
			 << wid;
	}
	
	int pid = XDOWrapper::xdo_get_pid_window(ctx, wid);
	if(!pid) {
		if(bDebug) {
			qDebug() << "WindowInfo::loop():: " 
				 << "Cannot get pid of window "
				 << wid
				 << ".";
		}
		return;
	}

	/// Read the command line of the program in focus to actually 
	/// verify if it's a supported program.
	QString cmdline;
	{
	   QString filename = QString::fromUtf8("/proc/%1/cmdline");
	   QFile file(filename.arg(pid));
	   if(!file.open(QIODevice::ReadOnly)) {
		   if(bDebug) {
			   qDebug() << "WindowInfo::loop():: "
				    << "Cannot open " << filename.arg(pid)
				    << ".";
		   }

		   emit hintHide();
		   return;
	   }

	   cmdline = QString(file.readAll());
	   file.close();
	}

	bool show = false;
	for(auto program : m_ProgramSigns) {
		if(cmdline.contains(program)) {
			if(program == "konsole") {
				emit yOffHint(48);
			} else if(program == "gnome-terminal") {
				emit yOffHint(0);
			} else if(program == "io.elementary.terminal") {
				emit yOffHint(0);
			}
			show = true;	
			break;
		}
	}

	if(show) {
		if(bDebug) {
			qDebug() << "WindowInfo::loop():: " 
				 << "PID(" << pid << "):: " 
				 << "Focused Window.";
		}
	
		int x = 0,
		    y = 0;
		unsigned w = 0,
			 h = 0;
		int ret  = 0;

		ret = XDOWrapper::xdo_get_window_location(ctx, wid, &x, &y, NULL);
		if(ret){
			if(bDebug) {
				qDebug() << "WindowInfo::loop():: " 
					<< "PID(" << pid << "):: " 
					<< "Cannot get Window location.";
			}
			emit unFocused();
			return;
		}
		ret = XDOWrapper::xdo_get_window_size(ctx, wid, &w, &h);
		if(ret) {
			if(bDebug) {
				qDebug() << "WindowInfo::loop():: " 
					<< "PID(" << pid << "):: " 
					<< "Cannot get Window szie.";
	
			}
			emit unFocused();
			return;
		}

		if(bDebug) {
			unsigned char *name = NULL;
			int name_len = 0;
			int name_type = 0;

			ret = XDOWrapper::xdo_get_window_name(
				ctx,
				wid,
				&name,
				&name_len,
				&name_type);

			QString title;

			if(!ret) {
				{
					QByteArray array((const char*)name, name_len);
					title = QString(array);

				}
				free(name);
			}

			qDebug() << "WindowInfo::loop():: " 
				 << "PID(" << pid << "):: " 
				 << "Title -> " 
				 << title
				 << ", X: "
				 << x 
				 << " Y: "
				 << y
				 << " Width: "
				 << w
				 << " Height: "
				 << h
				 << "." 
				 ;
		}

		emit focused(x,y,w,h);
	}else {
		if(bDebug) {
			qDebug() << "WindowInfo::loop():: " 
				 << "Focused Window pid is " << pid;
		}
		emit hintHide();
	}
}
