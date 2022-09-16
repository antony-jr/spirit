#ifndef SPIRIT_WORKER_HPP_INCLUDED
#define SPIRIT_WORKER_HPP_INCLUDED
#include <QRect>
#include <QJsonObject>
#include <QBuffer>
#include <QThread>

#include "spiritenums.hpp"

class SpiritWorkerPrivate;
class SpiritWorker : public QObject {
    Q_OBJECT
  public:
    struct Error : public SpiritEnums::Spirit::Error { };
    struct Status : public SpiritEnums::Spirit::Status { };

    SpiritWorker(QObject *parent = nullptr);
    ~SpiritWorker();

  public Q_SLOTS:
    void getActions();
    void getCurrentAction();
    void getInfo();

    void init();
    void cancel();

    /// Set the current .spirit file.
    void setSpiritFile(const QString&);

    /// Set the current action to be emitted.
    /// By default the default action will be running
    /// The action string should be valid and should present
    /// in the .spirit file.
    void setAction(const QString&);
  Q_SIGNALS:
    // Emits the current state of the worker.
    // Which could be,
    // 1. Idle
    // 2. Errored
    // 3. Loading
    // 4. Animating
    // 5. Stopped (Meaning the default action is not loop and stopped).
    // 6. Canceled (Stopped intentionally).
    void status(short);

    // Emitted when a spirit is initialized successfully.
    void initialized(QJsonObject);

    // Emitted when the status is animating.
    void started();

    // Emitted when call to cancel is successful.
    void canceled();

    // Emitted when some error in validating the
    // .spirit file occurs or some invalid action
    // was requested.
    void error(short);

    /// Emitted when called by getActions()
    void actions(QList<QString>);

    /// Emittted from getCurrentAction()
    /// Gives the current webp to
    /// to display in binary.
    /// Assume this binary is a webp file.
    void  action(QString /*current action name*/,
                 QBuffer* /*current webp file*/,
                 QBuffer* /*current webp variant*/,
                 QBuffer* /*audio file if given else nullptr*/,
                 bool /*loop*/,
                 int /*scale percentage*/,
                 int /*speed percentage*/,
                 QString /*next action if available*/,
                 QVector<int> /*offsets*/);

    /// Emits all the meta info on this
    /// .spirit file.
    void info(QJsonObject);

  private:
    QThread *m_WorkerThread;
    SpiritWorkerPrivate *m_Worker;
};

#endif // SPIRIT_WORKER_HPP_INCLUDED
