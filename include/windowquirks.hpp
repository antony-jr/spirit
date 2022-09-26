#ifndef WINDOW_QUIRKS_HPP_INCLUDED
#define WINDOW_QUIRKS_HPP_INCLUDED
#include <QObject>
#include <QMap>
#include <QPair>
#include <QJsonObject>
#include <iostream>
#include <cstdlib>

class WindowQuirks : public QObject {
    Q_OBJECT
  public:

    WindowQuirks(QObject *parent = nullptr);
    ~WindowQuirks();

  public Q_SLOTS:
    bool read();

    bool setGlobalXOffset(int, int);
    bool setGlobalYOffset(int, int);

    bool addQuirk(QString name, int x = 0, int y = 0,
                  int bottomX = 0, int bottomY = 0, QString visibleName = QString());
    bool removeQuirk(QString name);
    void getQuirks();

    QJsonObject getQuirk(QString name = QString());

  Q_SIGNALS:
    void quirks(QJsonObject);

  private:
    bool updateJson();

    int n_Y = 0,
        n_X = 0,
        n_BottomX = 0,
        n_BottomY = 0;

    struct Entry {
        int n_X = 0,
            n_Y = 0,
            n_BottomX = 0,
            n_BottomY = 0;
        QString m_VisibleName;
    };

    QString m_QuirkFilePath;
    QMap<QString, Entry> m_ProgramQuirks;
};

#endif
