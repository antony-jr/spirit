#ifndef ACTIVE_WINDOW_TRACKER_HPP_PRIVATE
#define ACTIVE_WINDOW_TRACKER_HPP_PRIVATE
#include <QObject>
#include <QRect>
#include <QStringList>
#ifdef Q_OS_LINUX
# include <kwindowsystem.h>
# include <kwindowinfo.h>
#endif // LINUX 

#include "spiritenums.hpp"
#include "windowquirks.hpp"

class ActiveWindowTrackerPrivate : public QObject {
    Q_OBJECT
  public:

    struct Error : public SpiritEnums::ActiveWindowTracker::Error { };

    ActiveWindowTrackerPrivate(QObject *parent = nullptr);
    ~ActiveWindowTrackerPrivate();
  public Q_SLOTS:
    void start();
    void rescan();
    void stop();

    void getQuirks();
    void addQuirk(QString, int, int, QString);
    void removeQuirk(QString);
    void setGlobalOffsets(int, int);

    void addAllowedProgram(QString);
    void removeAllowedProgram(int);
    void getAllowedPrograms();

#ifdef Q_OS_LINUX
  private Q_SLOTS:
    void updateActiveWindowX(WId);
    void handleWindowChanged(WId, NET::Properties, NET::Properties2);
    void handleWindowRemoved(WId);
    void handleWindowAdded(WId);
#endif // LINUX

  Q_SIGNALS:
    void quirks(QJsonObject);
    void quirkAdded(QString, bool);
    void quirkRemoved(QString, bool);
    void updatedGlobalOffsets(int, int);

    void error(short);
    void update(QRect, int, int);
    void hide();

    void allowedPrograms(QStringList);

  private:
    WindowQuirks m_Quirks;
    QStringList m_AllowedPrograms;
#ifdef Q_OS_LINUX
    bool b_RegisteredTypes = false;
#endif // LINUX
};

#endif //ACTIVE_WINDOW_TRACKER_HPP_PRIVATE
