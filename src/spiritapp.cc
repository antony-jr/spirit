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
    config = new SpiritConfig(this);
}

SpiritApp::~SpiritApp() {
    spirit->deleteLater();
}

bool SpiritApp::run() {
    if(!config->read()) {
        // TODO: handle this error better.
        emit quit();
        return false;
    }

    {
        auto programs = config->getAllowedPrograms();
        for (auto program : programs) {
            tracker->addAllowedProgram(program);
        }
    }

    QObject::connect(daemon, &SpiritDaemon::started, worker, &SpiritWorker::init);
    QObject::connect(daemon, &SpiritDaemon::quit, this, &SpiritApp::quit, Qt::DirectConnection);
    QObject::connect(tracker, &ActiveWindowTracker::hide, spirit, &Spirit::hide, Qt::QueuedConnection);
    QObject::connect(spirit, &Spirit::requestUpdate, tracker, &ActiveWindowTracker::rescan, Qt::QueuedConnection);
    QObject::connect(worker, &SpiritWorker::initialized, this, &SpiritApp::handleInit, Qt::QueuedConnection);
    QObject::connect(tracker, &ActiveWindowTracker::error,
                     this, &SpiritApp::handleTrackerError, Qt::QueuedConnection);
    QObject::connect(worker, &SpiritWorker::error, this, &SpiritApp::handleSpiritFileError, Qt::QueuedConnection);
    QObject::connect(daemon, &SpiritDaemon::setAction, worker, &SpiritWorker::setAction);
    QObject::connect(worker, &SpiritWorker::action, this, &SpiritApp::handleActionChange, Qt::QueuedConnection);
    QObject::connect(daemon, &SpiritDaemon::setSpirit, this, &SpiritApp::handleSpiritFile, Qt::QueuedConnection);
    QObject::connect(daemon, &SpiritDaemon::setPosition, spirit, &Spirit::setPosition);
    QObject::connect(daemon, &SpiritDaemon::setScale, spirit, &Spirit::setScale);
    QObject::connect(daemon, &SpiritDaemon::setSpeed, spirit, &Spirit::setSpeed);
    QObject::connect(daemon, &SpiritDaemon::setXOffset, spirit, &Spirit::setXOffset);
    QObject::connect(daemon, &SpiritDaemon::setYOffset, spirit, &Spirit::setYOffset);
    QObject::connect(daemon, &SpiritDaemon::setGlobalOffsets, tracker, &ActiveWindowTracker::setGlobalOffsets);
    QObject::connect(daemon, &SpiritDaemon::addQuirk, tracker, &ActiveWindowTracker::addQuirk);
    QObject::connect(daemon, &SpiritDaemon::removeQuirk, tracker, &ActiveWindowTracker::removeQuirk);
    QObject::connect(daemon, &SpiritDaemon::requestQuirks, tracker, &ActiveWindowTracker::getQuirks, Qt::QueuedConnection);
    QObject::connect(tracker, &ActiveWindowTracker::quirks, daemon, &SpiritDaemon::updateQuirks);

    QObject::connect(daemon, &SpiritDaemon::requestLatestProperties, spirit, &Spirit::getProperties,
                     Qt::QueuedConnection);
    QObject::connect(daemon, &SpiritDaemon::resetProperties, spirit, &Spirit::resetDefaults, Qt::QueuedConnection);
    QObject::connect(spirit, &Spirit::properties, daemon, &SpiritDaemon::updateProps, Qt::QueuedConnection);

    worker->setSpiritFile(config->getDefaultSpiritFile());
    tracker->start();
    daemon->run();

    return true;
}


// Private Slots
#include <QDebug>

void SpiritApp::handleInit(QJsonObject info) {
    daemon->updateSpiritMeta(info);

    QObject::connect(tracker, &ActiveWindowTracker::update, spirit, &Spirit::update, Qt::QueuedConnection);
    QObject::connect(worker, &SpiritWorker::action, spirit, &Spirit::animate, Qt::QueuedConnection);
    QObject::connect(spirit, &Spirit::requestAction, worker, &SpiritWorker::setAction, Qt::QueuedConnection);
    QObject::connect(tracker, &ActiveWindowTracker::hide, spirit, &Spirit::hide, Qt::QueuedConnection);

    worker->getCurrentAction();
}

void SpiritApp::handleActionChange(QString action) {
    QEventLoop loop;
    QList<QString> actions;

    auto conn = QObject::connect(worker, &SpiritWorker::actions,
    [&actions, &loop](QList<QString> acts) {
        actions = acts;
        loop.quit();
    });
    worker->getActions();
    loop.exec();

    QObject::disconnect(conn);
    daemon->updateAction(action, actions);
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
