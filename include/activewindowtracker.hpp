#ifndef ACTIVE_WINDOW_TRACKER_HPP
#define ACTIVE_WINDOW_TRACKER_HPP
#include <QObject>
#include <QJsonObject>
#include <QStringList>
#include <QRect>

#include "spiritenums.hpp"

class QThread;
class ActiveWindowTrackerPrivate;
class ActiveWindowTracker : public QObject {
    Q_OBJECT
  public:
    struct Error : public SpiritEnums::ActiveWindowTracker::Error { };

    ActiveWindowTracker(QObject *parent = nullptr);
    ~ActiveWindowTracker();
  Q_SIGNALS:
    void error(short);
    void update(QRect, int, int, int, int);
    void hide();

    void quirks(QJsonObject);
    void quirkAdded(QString, bool);
    void quirkRemoved(QString, bool);
    void allowedPrograms(QStringList);
  public Q_SLOTS:
    void getQuirks();
    void setGlobalOffsets(int, int, int, int);
    void addQuirk(QString, int, int, int, int, QString);
    void removeQuirk(QString);

    void start();
    void rescan();
    void stop();

    void addAllowedProgram(QString);
    void removeAllowedProgram(int);
    void getAllowedPrograms();
  private:
    ActiveWindowTrackerPrivate *d;
    QThread *worker;
};

#endif // ACTIVE_WINDOW_TRACKER_HPP
