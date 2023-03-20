#ifndef ACTIVE_WINDOW_TRACKER_HPP_PRIVATE
#define ACTIVE_WINDOW_TRACKER_HPP_PRIVATE
#include <QObject>
#include <QRect>
#include <QStringList>
#include <QTimer>
#ifdef Q_OS_LINUX
# include <QX11Info>
# include <kwindowsystem.h>
# include <kwindowinfo.h>
# include <netwm.h>
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

  private Q_SLOTS:
    void updateActiveWindow();

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

    QTimer *m_WindowTimer;
};

#endif //ACTIVE_WINDOW_TRACKER_HPP_PRIVATE
