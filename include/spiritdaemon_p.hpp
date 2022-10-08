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
    void updateQuirks(QJsonObject);

  Q_SIGNALS:
    void started();
    void cachedAction();
    void cachedSpirit();
    void cachedSpiritMeta();
    void cachedProps();
    void cachedQuirks();

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
    void addQuirk(QString, int, int, int, int, QString);
    void removeQuirk(QString);
    void setGlobalOffsets(int, int, int, int);
    void setXOffset(int, int);
    void setYOffset(int, int);
    void setPosition(short);
    void setScale(int);
    void setSpeed(int);

  private:
    bool b_StopRequested = false;

  public:
    bool b_QuitRequested = false;
    QJsonObject m_Meta;
    QJsonObject m_Quirks;
    int n_X = 0,
        n_Y = 0, // Global Quirks.
        n_BottomX = 0,
        n_BottomY = 0;

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
