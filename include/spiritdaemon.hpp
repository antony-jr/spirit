#ifndef SPIRIT_DAEMON_HPP_INCLUDED
#define SPIRIT_DAEMON_HPP_INCLUDED
#include <QThread>
#include <QJsonObject>

#include "spiritenums.hpp"

class SpiritDaemonPrivate;
class SpiritDaemon : public QObject {
    Q_OBJECT
  public:
    SpiritDaemon(QObject *parent = nullptr);
    ~SpiritDaemon();

  public Q_SLOTS:
    void run();
    void stop();

    void updateAction(QString);
    void updateSpirit(QString);
    void updateSpiritMeta(QJsonObject);

  Q_SIGNALS:
    void started();
    void quit();

    void pause();
    void resume();
    void setAction(QString);
    void resetAction();
    void setSpirit(QString);
    void unsetSpirit();
    void setPosition(short);
    void setScale(int);
    void setSpeed(int);


  private:
    QThread *m_WorkerThread;
    SpiritDaemonPrivate *m_Worker;
};

#endif // SPIRIT_DAEMON_HPP_INCLUDED
