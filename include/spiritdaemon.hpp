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

    void updateAction(QString, QList<QString>);
    void updateSpirit(QString);
    void updateSpiritMeta(QJsonObject);
    void updateProps(int x1, int x2,
                     int y1, int y2,
                     int scale, int speed,
                     int position,
                     QString sign);
    void updateQuirks(QJsonObject);

  Q_SIGNALS:
    void started();
    void quit();

    void pause();
    void resume();
    void setAction(QString);
    void resetAction();
    void setSpirit(QString);
    void unsetSpirit();
    void requestLatestProperties();
    void resetProperties();
    void requestQuirks();
    void addQuirk(QString, int, int, QString);
    void removeQuirk(QString);
    void setGlobalOffsets(int, int);
    void setXOffset(int, int);
    void setYOffset(int, int);
    void setPosition(short);
    void setScale(int);
    void setSpeed(int);


  private:
    QThread *m_WorkerThread;
    SpiritDaemonPrivate *m_Worker;
};

#endif // SPIRIT_DAEMON_HPP_INCLUDED
