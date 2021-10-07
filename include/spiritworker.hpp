#ifndef SPIRIT_WORKER_HPP_INCLUDED
#define SPIRIT_WORKER_HPP_INCLUDED
#include <QRect>
#include <QWidget>
#include <QThread>
#include <QScopedPointer>

#include "spirtworker_p.hpp"
#include "spiritenums.hpp"

class SpiritWorker : public QObject {
	Q_OBJECT
public:
	struct Error : SpiritEnums::Spirit::Error { };
	struct Status : SpiritEnums::Spirit::Status { };

	SpiritWorker(QObject *parent = nullptr);
	~SpiritWorker();

public Q_SLOTS:
        /// Initialize the .spirt file into memory,
	/// validate it and start giving frames for the
	/// default action.   
   	void start();

	/// Cancel the current .spirit file.
	void cancel();

	/// Set the current .spirit file.
	void setSpiritFile(const QString&);

	/// Set the current action to be emitted.
	/// By default the default action will be running
	/// The action string should be valid and should present
	/// in the .spirit file.
	void setAction(const QString&);

	void getActions();
	void getCurrentAction();
	void getInfo();
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

	// Emitted when the status is animating.
	// It also gives the same json object
	// when getInfo() is called.	
	void started(QJsonObject);

	// Emitted when call to cancel is successful.
	void canceled();

	// Emitted when some error in validating the 
	// .spirit file occurs or some invalid action
	// was requested.
	void error(short);

	// Emits the the frame to display.
	void frame(QBuffer*);

	/// Emitted when called by getActions()
	void actions(QList<QString>);

	/// Emittted from getCurrentAction()
	void action(QString);

	/// Emitted whenever the action is changed 
	/// for the loaded .spirit file.
	void actionChanged(QString);

	/// Emits all the meta info on this
	/// .spirit file.
	void info(QJsonObject);

private:
	QScopedPointer<QThread> m_WorkerThread;
	QScopedPointer<SpiritWorkerPrivate> m_Worker;
};

#endif // SPIRIT_WORKER_HPP_INCLUDED
