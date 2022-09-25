#include "spiritdaemon.hpp"
#include "spiritdaemon_p.hpp"
#include "helpers_p.hpp"

SpiritDaemon::SpiritDaemon(QObject *parent)
    : QObject(parent) {
    m_WorkerThread = new QThread;
    m_WorkerThread->start();

    m_Worker = new SpiritDaemonPrivate;
    m_Worker->moveToThread(m_WorkerThread);


    connect(m_Worker, &SpiritDaemonPrivate::started,
            this, &SpiritDaemon::started, Qt::DirectConnection);

    connect(m_Worker, &SpiritDaemonPrivate::quit,
            this, &SpiritDaemon::quit, Qt::DirectConnection);

    connect(m_Worker, &SpiritDaemonPrivate::pause,
            this, &SpiritDaemon::pause, Qt::DirectConnection);
    connect(m_Worker, &SpiritDaemonPrivate::resume,
            this, &SpiritDaemon::resume, Qt::DirectConnection);

    connect(m_Worker, &SpiritDaemonPrivate::setAction,
            this, &SpiritDaemon::setAction, Qt::DirectConnection);

    connect(m_Worker, &SpiritDaemonPrivate::resetAction,
            this, &SpiritDaemon::resetAction, Qt::DirectConnection);

    connect(m_Worker, &SpiritDaemonPrivate::setSpirit,
            this, &SpiritDaemon::setSpirit, Qt::DirectConnection);

    connect(m_Worker, &SpiritDaemonPrivate::unsetSpirit,
            this, &SpiritDaemon::unsetSpirit, Qt::DirectConnection);

    connect(m_Worker, &SpiritDaemonPrivate::requestLatestProperties,
            this, &SpiritDaemon::requestLatestProperties, Qt::DirectConnection);

    connect(m_Worker, &SpiritDaemonPrivate::resetProperties,
            this, &SpiritDaemon::resetProperties, Qt::DirectConnection);

    connect(m_Worker, &SpiritDaemonPrivate::requestQuirks,
            this, &SpiritDaemon::requestQuirks, Qt::DirectConnection);

    connect(m_Worker, &SpiritDaemonPrivate::addQuirk,
            this, &SpiritDaemon::addQuirk, Qt::DirectConnection);

    connect(m_Worker, &SpiritDaemonPrivate::removeQuirk,
            this, &SpiritDaemon::removeQuirk, Qt::DirectConnection);

    connect(m_Worker, &SpiritDaemonPrivate::setGlobalOffsets,
            this, &SpiritDaemon::setGlobalOffsets, Qt::DirectConnection);

    connect(m_Worker, &SpiritDaemonPrivate::setXOffset,
            this, &SpiritDaemon::setXOffset, Qt::DirectConnection);

    connect(m_Worker, &SpiritDaemonPrivate::setYOffset,
            this, &SpiritDaemon::setYOffset, Qt::DirectConnection);

    connect(m_Worker, &SpiritDaemonPrivate::setPosition,
            this, &SpiritDaemon::setPosition, Qt::DirectConnection);

    connect(m_Worker, &SpiritDaemonPrivate::setScale,
            this, &SpiritDaemon::setScale, Qt::DirectConnection);

    connect(m_Worker, &SpiritDaemonPrivate::setSpeed,
            this, &SpiritDaemon::setSpeed, Qt::DirectConnection);


}

SpiritDaemon::~SpiritDaemon() {
    m_Worker->disconnect();
    stop();

    m_WorkerThread->quit();
    m_WorkerThread->wait();

    m_Worker->deleteLater();
    m_WorkerThread->deleteLater();
}

void SpiritDaemon::run() {
    getMethod(m_Worker, "run()")
    .invoke(m_Worker,
            Qt::QueuedConnection);
}

void SpiritDaemon::stop() {
    getMethod(m_Worker, "stop()")
    .invoke(m_Worker,
            Qt::QueuedConnection);
}

void SpiritDaemon::updateAction(QString action, QList<QString> actions) {
    getMethod(m_Worker, "updateAction(QString, QList<QString>)")
    .invoke(m_Worker,
            Qt::QueuedConnection, Q_ARG(QString, action), Q_ARG(QList<QString>, actions));
}

void SpiritDaemon::updateSpirit(QString spirit) {
    getMethod(m_Worker, "updateSpirit(QString)")
    .invoke(m_Worker,
            Qt::QueuedConnection, Q_ARG(QString, spirit));
}

void SpiritDaemon::updateSpiritMeta(QJsonObject meta) {
    getMethod(m_Worker, "updateSpiritMeta(QJsonObject)")
    .invoke(m_Worker,
            Qt::QueuedConnection, Q_ARG(QJsonObject, meta));
}

void SpiritDaemon::updateQuirks(QJsonObject quirks) {
    getMethod(m_Worker, "updateQuirks(QJsonObject)")
    .invoke(m_Worker,
            Qt::QueuedConnection, Q_ARG(QJsonObject, quirks));
}

void SpiritDaemon::updateProps(
    int x1, int x2,
    int y1, int y2,
    int scale, int speed,
    int position,
    QString signature
) {
    getMethod(m_Worker, "updateProps(int,int,int,int,int,int,int,QString)")
    .invoke(m_Worker,
            Qt::QueuedConnection, Q_ARG(int, x1), Q_ARG(int, x2),
            Q_ARG(int, y1), Q_ARG(int, y2),
            Q_ARG(int, scale), Q_ARG(int, speed),
            Q_ARG(int, position),
            Q_ARG(QString, signature));
}
