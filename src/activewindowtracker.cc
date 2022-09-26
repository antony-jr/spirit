#include <QThread>

#include "helpers_p.hpp"
#include "activewindowtracker_p.hpp"
#include "activewindowtracker.hpp"

ActiveWindowTracker::ActiveWindowTracker(QObject *parent)
    : QObject(parent),
      d(new ActiveWindowTrackerPrivate),
      worker(new QThread) {
    worker->start();
    d->moveToThread(worker);

    connect(d, &ActiveWindowTrackerPrivate::error,
            this, &ActiveWindowTracker::error,
            Qt::DirectConnection);

    connect(d, &ActiveWindowTrackerPrivate::update,
            this, &ActiveWindowTracker::update,
            Qt::DirectConnection);

    connect(d, &ActiveWindowTrackerPrivate::hide,
            this, &ActiveWindowTracker::hide,
            Qt::DirectConnection);

    connect(d, &ActiveWindowTrackerPrivate::quirks,
            this, &ActiveWindowTracker::quirks,
            Qt::DirectConnection);

    connect(d, &ActiveWindowTrackerPrivate::quirkAdded,
            this, &ActiveWindowTracker::quirkAdded,
            Qt::DirectConnection);

    connect(d, &ActiveWindowTrackerPrivate::quirkRemoved,
            this, &ActiveWindowTracker::quirkRemoved,
            Qt::DirectConnection);

    connect(d, &ActiveWindowTrackerPrivate::allowedPrograms,
            this, &ActiveWindowTracker::allowedPrograms,
            Qt::DirectConnection);
}

ActiveWindowTracker::~ActiveWindowTracker() {
    stop();
    d->deleteLater();

    worker->quit();
    worker->wait();
    worker->deleteLater();
}

void ActiveWindowTracker::getQuirks() {
    getMethod(d, "getQuirks()").invoke(d, Qt::QueuedConnection);
}

void ActiveWindowTracker::setGlobalOffsets(int x, int y, int x2, int y2) {
    getMethod(d, "setGlobalOffsets(int, int)")
    .invoke(d, Qt::QueuedConnection,
            Q_ARG(int, x),
            Q_ARG(int, y),
            Q_ARG(int, x2),
            Q_ARG(int, y2));
}

void ActiveWindowTracker::addQuirk(QString name, int x, int y, int x2, int y2, QString visibleName) {
    getMethod(d, "addQuirk(QString, int, int, int, int, QString)")
    .invoke(d, Qt::QueuedConnection,
            Q_ARG(QString, name),
            Q_ARG(int, x),
            Q_ARG(int, y),
            Q_ARG(int, x2),
            Q_ARG(int, y2),
            Q_ARG(QString, visibleName));
}

void ActiveWindowTracker::removeQuirk(QString name) {
    getMethod(d, "removeQuirk(QString)").invoke(d, Qt::QueuedConnection, Q_ARG(QString, name));
}

void ActiveWindowTracker::start() {
    getMethod(d, "start()").invoke(d, Qt::QueuedConnection);
}

void ActiveWindowTracker::rescan() {
    getMethod(d, "rescan()").invoke(d, Qt::QueuedConnection);
}

void ActiveWindowTracker::stop() {
    getMethod(d, "stop()").invoke(d, Qt::QueuedConnection);
}

void ActiveWindowTracker::addAllowedProgram(QString program) {
    getMethod(d, "addAllowedProgram(QString)").invoke(d, Qt::QueuedConnection, Q_ARG(QString, program));
}

void ActiveWindowTracker::removeAllowedProgram(int pos) {
    getMethod(d, "removeAllowedProgram(int)").invoke(d, Qt::QueuedConnection, Q_ARG(int, pos));
}

void ActiveWindowTracker::getAllowedPrograms() {
    getMethod(d, "getAllowedPrograms()").invoke(d, Qt::QueuedConnection);
}
