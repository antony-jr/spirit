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
}

ActiveWindowTracker::~ActiveWindowTracker() {
    stop();
    d->deleteLater();

    worker->quit();
    worker->wait();
    worker->deleteLater();
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

