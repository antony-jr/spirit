#ifndef ACTIVE_WINDOW_TRACKER_HPP_PRIVATE
#define ACTIVE_WINDOW_TRACKER_HPP_PRIVATE
#include <QObject>
#include <QRect>
#include <QStringList>
#include <QTimer>
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
    void addQuirk(QString, int, int, int, int, QString);
    void removeQuirk(QString);
    void setGlobalOffsets(int, int, int, int);

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

#if defined(Q_OS_WINDOWS) || defined(Q_OS_MAC)
  private Q_SLOTS:
    void updateActiveWindow();
#endif // WINDOWS || MAC


  Q_SIGNALS:
    void quirks(QJsonObject);
    void quirkAdded(QString, bool);
    void quirkRemoved(QString, bool);

    void error(short);
    void update(QRect, int, int, int, int);
    void hide();

    void allowedPrograms(QStringList);

  private:
    WindowQuirks m_Quirks;
    QStringList m_AllowedPrograms;
#ifdef Q_OS_LINUX
    bool b_RegisteredTypes = false;
#endif // LINUX
       //
#if defined(Q_OS_WINDOWS) || defined(Q_OS_MAC)
    QTimer *m_WindowTimer;
#endif // WINDOWS || MAC
};

#endif //ACTIVE_WINDOW_TRACKER_HPP_PRIVATE
