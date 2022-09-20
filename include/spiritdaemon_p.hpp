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

    void updateAction(QString, QList<QString>);
    void updateSpirit(QString);
    void updateSpiritMeta(QJsonObject);
    void updateProps(int x1, int x2,
                     int y1, int y2,
                     int scale, int speed,
                     int position,
                     QString sign);

  Q_SIGNALS:
    void started();
    void cachedAction();
    void cachedSpirit();
    void cachedSpiritMeta();
    void cachedProps();

    void quit();
    void pause();
    void resume();
    void setAction(QString);
    void resetAction();
    void setSpirit(QString);
    void unsetSpirit();
    void requestLatestProperties();
    void resetProperties();
    void setXOffset(int, int);
    void setYOffset(int, int);
    void setPosition(short);
    void setScale(int);
    void setSpeed(int);

  private:
    bool b_StopRequested = false;

  public:
    QJsonObject m_Meta;

    QList<QString> m_ActionList;
    QString m_Action,
            m_Spirit;
    short m_Position = SpiritEnums::Spirit::Position::TopLeft;
    int n_Scale = 100,
        n_Speed = 100;

    int n_x1 = 0,
        n_x2 = 0,
        n_y1 = 0,
        n_y2 = 0;
    QString m_Sign;
};

#endif // SPIRIT_DAEMON_PRIVATE_HPP_INCLUDED
