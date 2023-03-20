#include <QRect>
#include <QFile>
#include <QFileInfo>
#include <QDebug>

#include "activewindowtracker_p.hpp"

#ifdef Q_OS_MAC
extern "C" {
#include "macos_p.h"
}
#endif // MAC

#ifdef Q_OS_WINDOWS
# include <windows.h>
# include <psapi.h>
#endif // WINDOWS

ActiveWindowTrackerPrivate::ActiveWindowTrackerPrivate(QObject *parent)
    : QObject(parent) {
    QObject::connect(&m_Quirks, &WindowQuirks::quirks,
                     this, &ActiveWindowTrackerPrivate::quirks,
                     Qt::DirectConnection);

    m_Quirks.read();

    m_WindowTimer = new QTimer(this);
    m_WindowTimer->setSingleShot(false);
    m_WindowTimer->setInterval(50);

    connect(m_WindowTimer, &QTimer::timeout,
            this, &ActiveWindowTrackerPrivate::updateActiveWindow);
}

ActiveWindowTrackerPrivate::~ActiveWindowTrackerPrivate() {
    stop();
}

void ActiveWindowTrackerPrivate::getQuirks() {
    m_Quirks.getQuirks();
}

void ActiveWindowTrackerPrivate::addQuirk(QString name, int x, int y, int x2, int y2, QString vname) {
    auto ok = m_Quirks.addQuirk(name, x, y, x2, y2, vname);
    emit quirkAdded(name, ok);
}

void ActiveWindowTrackerPrivate::removeQuirk(QString name) {
    auto ok = m_Quirks.removeQuirk(name);
    emit quirkRemoved(name, ok);
}

void ActiveWindowTrackerPrivate::setGlobalOffsets(int x, int y, int x2, int y2) {
    m_Quirks.setGlobalXOffset(x, x2);
    m_Quirks.setGlobalYOffset(y, y2);

    getQuirks();
}

void ActiveWindowTrackerPrivate::start() {
#ifdef Q_OS_LINUX
    if(KWindowSystem::platform() != KWindowSystem::Platform::X11) {
        /// We need to handle this specially since there is no common api
        /// to get active window in wayland, we might need to DE specific stuff
        /// which is very painful.
        /// For now we will only support GNOME (Wayland) since it supports getting
        /// active window via gdbus command.

        emit error(Error::UnSupportedPlatform);
        return;
    }
#endif // LINUX

    // Start the loop to track active window.
    m_WindowTimer->start();
}

void ActiveWindowTrackerPrivate::rescan() {
#ifdef Q_OS_LINUX
    if(KWindowSystem::platform() != KWindowSystem::Platform::X11) {
        return;
    }
#endif // LINUX 

    updateActiveWindow();
}

void ActiveWindowTrackerPrivate::stop() {
    m_WindowTimer->stop();
}

void ActiveWindowTrackerPrivate::addAllowedProgram(QString program) {
    m_AllowedPrograms.append(program);
}

void ActiveWindowTrackerPrivate::removeAllowedProgram(int pos) {
    if (pos < m_AllowedPrograms.size() &&
            pos >= 0) {
        m_AllowedPrograms.removeAt(pos);
        emit allowedPrograms(m_AllowedPrograms);
    }
}

void ActiveWindowTrackerPrivate::getAllowedPrograms() {
    emit allowedPrograms(m_AllowedPrograms);
}

void ActiveWindowTrackerPrivate::updateActiveWindow() {
    QString title,
            clsName;
    QRect geo;

#ifdef Q_OS_LINUX
    QString name,
            clsClass;

    auto activeWindowId = KWindowSystem::activeWindow();
    if(activeWindowId == 0) {
        emit hide();
        return;
    }

    auto properties = NET::WMGeometry |
                      NET::WMState    |
                      NET::WMFrameExtents |
                      NET::WMName |
                      NET::WMVisibleName;

    auto props2 = NET::WM2WindowClass |
                  NET::WM2GTKFrameExtents;

    if (!QX11Info::connection()) {
        emit hide();
        return;
    }


    NETWinInfo wininfo(QX11Info::connection(),
                       activeWindowId,
                       QX11Info::appRootWindow(),
                       properties,
                       props2);

    auto state = wininfo.state();
    if(!(state & NET::State::Focused)) {
        emit hide();
        return;
    }

    if(state & NET::State::SkipTaskbar ||
            state & NET::State::Modal ||
            state & NET::State::KeepBelow) {
        emit hide();
        return;
    }

    name = QString(wininfo.name());
    title = wininfo.visibleName();
    clsName = QString(wininfo.windowClassName());
    clsClass = QString(wininfo.windowClassClass());

    auto extents = wininfo.gtkFrameExtents();

    NETRect frame;
    NETRect geom;

    wininfo.kdeGeometry(frame, geom);

    Q_UNUSED(frame);
    geo.setRect(frame.pos.x + extents.left,
                frame.pos.y + extents.top,
                frame.size.width - extents.right - extents.left,
                frame.size.height - extents.bottom - extents.top);

#endif // LINUX

#ifdef Q_OS_WINDOWS
    QString program;
    HWND foreground = GetForegroundWindow();
    if (foreground) {
        char window_title[256];
        GetWindowText(foreground, window_title, 256);
        title = QString(window_title);
    } else {
        hide();
        return;
    }

    DWORD wStyle;
    wStyle = GetWindowLong (foreground,GWL_STYLE);
    if(!(wStyle & WS_CAPTION)) {
        emit hide();
        return;
    }

    DWORD dwPID;
    GetWindowThreadProcessId(foreground, &dwPID);

    HANDLE handle = OpenProcess(
                        PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                        FALSE,
                        dwPID);

    if (handle) {
        char prog_path[256];
        if (GetProcessImageFileName(handle,
                                    prog_path,
                                    sizeof(prog_path)/sizeof(*prog_path)) !=0 ) {
            program = QString(prog_path);
        }
    }

    WINDOWINFO info { NULL };
    if (GetWindowInfo(foreground, &info)) {
        auto rect = info.rcClient;
        auto window_rect = RECT { NULL };

        if(!GetWindowRect(foreground, &window_rect)) {
            emit hide();
            return;

        }

        auto client_height = rect.bottom - rect.top + 1;
        auto title_bar_height = rect.top - window_rect.top + 1;
        auto height = client_height + title_bar_height;
        auto width = window_rect.right - window_rect.left + 1;

        int x = window_rect.left,
            y = window_rect.top;

        geo = QRect(x, y, width, height);
    } else {
        emit hide();
        return;
    }

    clsName =  QFileInfo(program).baseName();
#endif // WINDOWS

#ifdef Q_OS_MAC
    {
        int x,y,w,h;
        x = y = w = h = 0;
        char *window_title = nullptr;

        macos_get_active_window(&x, &y, &w, &h, &window_title);

        if (x == -1 && y == -1 &&
                w == -1 && h == -1 &&
                window_title == NULL) {
            emit hide();
            return;
        }

        geo = QRect(x, y, w, h);
        title = QString(window_title);
        free(window_title);
    }
    clsName = title;
#endif // MACOS

    auto allowed = m_AllowedPrograms.isEmpty();

    for (auto prog : m_AllowedPrograms) {
        if (title.contains(prog) ||
#ifdef Q_OS_LINUX
                name.contains(prog) ||
                clsClass.contains(prog) ||
#endif // LINUX
                clsName.contains(prog)) {
            allowed = true;
            break;
        }
    }

    if (!allowed) {
        emit hide();
        return;
    }

    auto quirk = m_Quirks.getQuirk(clsName);

#ifdef Q_OS_LINUX
    if (quirk["exists"].toBool() == false) {
        quirk = m_Quirks.getQuirk(clsClass);
    }
#endif // LINUX

    auto xoffset = quirk["xoffset"].toInt(0);
    auto yoffset = quirk["yoffset"].toInt(0);
    auto _xoffset = quirk["bottomXOffset"].toInt(0);
    auto _yoffset = quirk["bottomYOffset"].toInt(0);
    auto vname = quirk["visibleName"].toString();

#ifdef Q_OS_LINUX
    if (state & NET::State::Max) {
        _yoffset = _xoffset = 0;
    }
#endif // LINUX

    if (vname.isEmpty()) {
        emit update(geo, xoffset, yoffset, _xoffset, _yoffset);
    } else if (title.contains(vname)) {
        emit update(geo, xoffset, yoffset, _xoffset, _yoffset);
    } else {
        auto gl = m_Quirks.getQuirk();
        gl = gl["global"].toObject();

        int glx = gl["xoffset"].toInt(0);
        int gly = gl["yoffset"].toInt(0);
        auto gl_x = gl["bottomXOffset"].toInt(0);
        auto gl_y = gl["bottomYOffset"].toInt(0);

#ifdef Q_OS_LINUX
        if (state & NET::State::Max) {
            gl_y = gl_x = 0;
        }
#endif // LINUX

        emit update(geo, glx, gly, gl_x, gl_y);
    }
}
