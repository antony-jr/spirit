#include <QMessageBox>

#include "spiritapp.hpp"

SpiritApp::SpiritApp(QObject *parent) :
    QObject(parent) {
    // This is the QLabel which displays the
    // webp file over the screen, this should not
    // have any parent since it might inherit something.
    spirit = new Spirit();

    tracker = new ActiveWindowTracker(this);
    worker = new SpiritWorker(this);
    daemon = new SpiritDaemon(this);
}

SpiritApp::~SpiritApp() {
    spirit->deleteLater();
}

void SpiritApp::run() {
    // TODO: Load Configuration File
    // spirit->setDefaultScale()
    // spirit->setDefaultSpeed();
    // Get Default Spirit File location
    // Get List of Allowed Programs to Integrate
    // Add the list to ActiveWindowTracker

    QObject::connect(daemon, &SpiritDaemon::started, worker, &SpiritWorker::init);
    QObject::connect(daemon, &SpiritDaemon::quit, this, &SpiritApp::quit, Qt::DirectConnection);
    QObject::connect(tracker, &ActiveWindowTracker::hide, spirit, &Spirit::hide, Qt::QueuedConnection);
    QObject::connect(spirit, &Spirit::requestUpdate, tracker, &ActiveWindowTracker::rescan, Qt::QueuedConnection);
    QObject::connect(worker, &SpiritWorker::initialized, this, &SpiritApp::handleInit, Qt::QueuedConnection);
    QObject::connect(tracker, &ActiveWindowTracker::error,
                     this, &SpiritApp::handleTrackerError, Qt::QueuedConnection);
    QObject::connect(worker, &SpiritWorker::error, this, &SpiritApp::handleSpiritFileError, Qt::QueuedConnection);
    QObject::connect(daemon, &SpiritDaemon::setAction, worker, &SpiritWorker::setAction);
    QObject::connect(worker, &SpiritWorker::action, daemon, &SpiritDaemon::updateAction);
    QObject::connect(daemon, &SpiritDaemon::setSpirit, this, &SpiritApp::handleSpiritFile, Qt::QueuedConnection);
    QObject::connect(daemon, &SpiritDaemon::setPosition, spirit, &Spirit::setPosition);
    QObject::connect(daemon, &SpiritDaemon::setScale, spirit, &Spirit::setScale);
    QObject::connect(daemon, &SpiritDaemon::setSpeed, spirit, &Spirit::setSpeed);

    worker->setSpiritFile(":default.spirit");
    tracker->start();
    daemon->run();
}


// Private Slots
#include <QDebug>

void SpiritApp::handleInit(QJsonObject info) {
    qDebug() << "Init";
    daemon->updateSpiritMeta(info);

    QObject::connect(tracker, &ActiveWindowTracker::update, spirit, &Spirit::update, Qt::QueuedConnection);
    QObject::connect(worker, &SpiritWorker::action, spirit, &Spirit::animate, Qt::QueuedConnection);
    QObject::connect(spirit, &Spirit::requestAction, worker, &SpiritWorker::setAction, Qt::QueuedConnection);
    QObject::connect(tracker, &ActiveWindowTracker::hide, spirit, &Spirit::hide, Qt::QueuedConnection);

    worker->getCurrentAction();
}

void SpiritApp::handleSpiritFile(QString location) {
    QObject::disconnect(tracker, &ActiveWindowTracker::update, spirit, &Spirit::update);
    QObject::disconnect(worker, &SpiritWorker::action, spirit, &Spirit::animate);
    QObject::disconnect(spirit, &Spirit::requestAction, worker, &SpiritWorker::setAction);
    QObject::disconnect(tracker, &ActiveWindowTracker::hide, spirit, &Spirit::hide);

    spirit->clear();
    daemon->updateSpirit(location);

    QEventLoop loop;
    QObject::connect(worker, &SpiritWorker::status, &loop, &QEventLoop::quit);

    worker->cancel();
    loop.exec();

    worker->setSpiritFile(location);
    worker->init();
}


void SpiritApp::handleTrackerError(short code) {
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setWindowTitle("Spirit");

    switch (code) {
    case ActiveWindowTracker::Error::UnSupportedPlatform:
        msgBox.setText("Spirit is not currently supported in this platform.");
        break;
    default:
        msgBox.setText("Spirit failed to run because of an unknown error.");
        break;
    }

    msgBox.exec();
    emit quit();
}

void SpiritApp::handleSpiritFileError(short code) {
    // TODO: Improve this.
    Q_UNUSED(code);

    handleSpiritFile(":default.spirit");
}
