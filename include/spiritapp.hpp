#ifndef SPIRIT_APP_HPP_INCLUDED
#define SPIRIT_APP_HPP_INCLUDED
#include <QString>
#include <QJsonObject>
#include <QObject>

#include "activewindowtracker.hpp"
#include "spiritworker.hpp"
#include "spirit.hpp"
#include "spiritdaemon.hpp"
#include "spiritconfig.hpp"

class SpiritApp : public QObject {
    Q_OBJECT
  public:

    SpiritApp(QObject *parent = nullptr);
    ~SpiritApp();

  public Q_SLOTS:
    void run();

  private Q_SLOTS:
    void handleInit(QJsonObject);
    void handleActionChange(QString);
    void handleSpiritFile(QString);
    void handleTrackerError(short);
    void handleSpiritFileError(short);

  Q_SIGNALS:
    void quit();

  private:
    ActiveWindowTracker *tracker;
    Spirit *spirit;
    SpiritWorker *worker;
    SpiritDaemon *daemon;
    SpiritConfig *config;
};

#endif // SPIRIT_APP_HPP_INCLUDED
