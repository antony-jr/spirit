#ifndef SPIRIT_SETTINGS_HPP_INCLUDED
#define SPIRIT_SETTINGS_HPP_INCLUDED
#include <QObject>
#include <QJsonObject>
#include <QFile>
#include <QPair>

class SpiritSettings : public QObject {
    Q_OBJECT

  public:
    SpiritSettings(QObject *parent = nullptr);
    ~SpiritSettings();

  public Q_SLOTS:
    bool setSpiritSignature(QString,
                            int x1, int x2,
                            int y1, int y2,
                            int scale,
                            int speed,
                            short position);
    bool resetDefaults();

    bool setXOffset(int x1, int x2 = -1);
    bool setYOffset(int y1, int y2 = -1);
    bool setScale(int);
    bool setSpeed(int);
    bool setPosition(short);

    QPair<int, int> getXOffset();
    QPair<int, int> getYOffset();
    int getScale();
    int getSpeed();
    short getPosition();

  private:
    bool updateJson();

    QFile m_Config;
    QString m_SettingsDir;
    QString m_Sign;

    QJsonObject m_DefaultValues;
    QPair<int, int> m_XOffset,
          m_YOffset;
    int m_Scale = 100,
        m_Speed = 100,
        m_Position = 0;
};
#endif
