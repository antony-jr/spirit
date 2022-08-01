#ifndef SPIRIT_FILE_HPP_INCLUDED
#define SPIRIT_FILE_HPP_INCLUDED
#include <QRect>
#include <QJsonObject>
#include <QBuffer>
#include <QThread>

#include "spiritenums.hpp"

class SpiritFilePrivate;
class SpiritFile : public QObject {
	Q_OBJECT
public:
	struct Error : public SpiritEnums::SpiritFile::Error { };
	struct Status : public SpiritEnums::SpiritFile::Status { };
	
	SpiritFile(QObject *parent = nullptr);
	~SpiritFile();

public Q_SLOTS:
   	/// Initialize the .spirit file into memory,
	/// and validate it. 
   	void init();

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
	// 4. Loaded
	// 5. Canceled (Stopped intentionally).
	void status(short);

	// Emitted when the init is finished.
	// Also gives the same json object as 
	// get info.
	void initialized(QJsonObject);

	// Emitted when call to cancel is successful.
	void canceled();

	// Emitted when some error in validating the 
	// .spirit file occurs or some invalid action
	// was requested.
	void error(short);

	/// Emitted when called by getActions()
	void actions(QList<QString>);

	/// Emittted from getCurrentAction()
	void action(QString /*current action name*/, 
	      	     QBuffer* /*current webp file*/,
	      	     QBuffer* /*audio file if given*/,
	      	     bool /*loop*/,
		     int /*scale percentage*/,
		     int /*speed percentage*/,
		     QString /*next action if available*/);

	/// Emits all the meta info on this
	/// .spirit file.
	void info(QJsonObject);
private:
	QThread *m_WorkerThread;
	SpiritFilePrivate *m_Worker;
};

#endif // SPIRIT_FILE_HPP_INCLUDED
