#ifndef SPIRIT_DAEMON_PRIVATE_HPP_INCLUDED
#define SPIRIT_DAEMON_PRIVATE_HPP_INCLUDED
#include <QObject>
#include <QJsonObject>

#include "spiritenums.hpp"

class SpiritDaemonPrivate : public QObject {
    Q_OBJECT
  public:
    SpiritDaemonPrivate(QObject *parent = nullptr);
    ~SpiritDaemonPrivate();

  public Q_SLOTS:
    void run();
    void stop();

    void updateAction(QString);
    void updateSpirit(QString);
    void updateSpiritMeta(QJsonObject);

  Q_SIGNALS:
    void cachedAction();
    void cachedSpirit();
    void cachedSpiritMeta();

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
    bool b_StopRequested = false;

  public:
    QJsonObject m_Meta;

    QString m_Action,
            m_Spirit;
    short m_Position = SpiritEnums::Spirit::Position::TopLeft;
    int n_Scale = 100,
        n_Speed = 100;
};

#endif // SPIRIT_DAEMON_PRIVATE_HPP_INCLUDED
