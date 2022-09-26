#include <QRect>
#include <QFile>
#include <QFileInfo>
#include <QDebug>

#include "activewindowtracker_p.hpp"

ActiveWindowTrackerPrivate::ActiveWindowTrackerPrivate(QObject *parent)
    : QObject(parent) {
    QObject::connect(&m_Quirks, &WindowQuirks::quirks,
                     this, &ActiveWindowTrackerPrivate::quirks,
                     Qt::DirectConnection);

    m_Quirks.read();
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
    if(!b_RegisteredTypes) {
        qRegisterMetaType<WId>("WId");
        qRegisterMetaType<NET::Properties>("NET::Properties");
        qRegisterMetaType<NET::Properties2>("NET::Properties2");
        b_RegisteredTypes = true;
    }
    connect(KWindowSystem::self(),
            qOverload<WId, NET::Properties, NET::Properties2>(&KWindowSystem::windowChanged),
            this,
            &ActiveWindowTrackerPrivate::handleWindowChanged);

    connect(KWindowSystem::self(),
            &KWindowSystem::windowRemoved,
            this,
            &ActiveWindowTrackerPrivate::handleWindowRemoved);

    connect(KWindowSystem::self(),
            &KWindowSystem::windowAdded,
            this,
            &ActiveWindowTrackerPrivate::handleWindowAdded);

    // Initially no signal will be emitted so.
    handleWindowAdded(KWindowSystem::activeWindow());
#endif // LINUX
}

void ActiveWindowTrackerPrivate::rescan() {
#ifdef Q_OS_LINUX
    if(KWindowSystem::platform() == KWindowSystem::Platform::X11) {
        updateActiveWindowX(KWindowSystem::activeWindow());
    }
#endif // LINUX 
}

void ActiveWindowTrackerPrivate::stop() {
#ifdef Q_OS_LINUX
    this->disconnect();
#endif // LINUX

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

#ifdef Q_OS_LINUX
void ActiveWindowTrackerPrivate::updateActiveWindowX(WId id) {
    if(id == KWindowSystem::activeWindow()) {
        auto properties = NET::WMGeometry |
                          NET::WMState    |
                          NET::WMFrameExtents |
                          NET::WMName |
                          NET::WMVisibleName;

        auto props2 = NET::WM2WindowClass;

        KWindowInfo info(id, properties, props2);
        if(!info.valid()) {
            emit hide();
            return;
        }

        auto allowed = m_AllowedPrograms.isEmpty();
        auto name = info.name();
        auto title = info.visibleName();
        for (auto prog : m_AllowedPrograms) {
            if (title.contains(prog)) {
                allowed = true;
                break;
            }
        }

        if (!allowed) {
            emit hide();
            return;
        }

        /*
            QString program;
            {
                QString filename = QString::fromUtf8("/proc/%1/cmdline");
                QFile file(filename.arg(info.pid()));
                if(file.open(QIODevice::ReadOnly)) {
                    auto cmdline = QString(file.readAll());
                    program = QFileInfo(cmdline).baseName();
                    file.close();
                }
            }*/

        auto clsName = info.windowClassName();
        auto clsClass = info.windowClassClass();

        auto quirk = m_Quirks.getQuirk(clsName);
        if (quirk["exists"].toBool() == false) {
            quirk = m_Quirks.getQuirk(clsClass);
        }

        auto xoffset = quirk["xoffset"].toInt(0);
        auto yoffset = quirk["yoffset"].toInt(0);
        auto _xoffset = quirk["bottomXOffset"].toInt(0);
        auto _yoffset = quirk["bottomYOffset"].toInt(0);
        auto vname = quirk["visibleName"].toString();

        auto state = info.state();
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

        if (state & NET::State::Max) {
            _yoffset = _xoffset = 0;
        }

        auto geo = info.frameGeometry();

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

            if (state & NET::State::Max) {
                gl_y = gl_x = 0;
            }

            emit update(geo, glx, gly, gl_x, gl_y);
        }

        return;
    }

    updateActiveWindowX(KWindowSystem::activeWindow());
}

void ActiveWindowTrackerPrivate::handleWindowChanged(WId id,
        NET::Properties prop1,
        NET::Properties2 prop2) {
    Q_UNUSED(prop2);
    Q_UNUSED(prop1);
    updateActiveWindowX(id);
}

void ActiveWindowTrackerPrivate::handleWindowRemoved(WId id) {
    updateActiveWindowX(id);
}

void ActiveWindowTrackerPrivate::handleWindowAdded(WId id) {
    updateActiveWindowX(id);
}
#endif // LINUX
