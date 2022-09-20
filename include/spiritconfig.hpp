#ifndef SPIRIT_CONFIG_HPP_INCLUDED
#define SPIRIT_CONFIG_HPP_INCLUDED
#include <QObject>
#include <QFile>
#include <QStringList>

class SpiritConfig : public QObject {
    Q_OBJECT
  public:

    SpiritConfig(QObject *parent = nullptr);
    ~SpiritConfig();

  public Q_SLOTS:
    bool read();

    bool isRunOnStartup();
    QString getDefaultSpiritFile();
    QStringList getAllowedPrograms();

    bool setDefaultSpiritFile(QString);
    bool setRunOnStartup(bool);
    bool addAllowedProgram(QString);
    bool removeAllowedProgram(int);
  private:
    bool updateJson();

    QString m_DefaultSpiritFile = ":default.spirit",
            m_ConfigPath;
    QStringList m_AllowedPrograms;
    bool b_RunOnStartup = false;
};

#endif
