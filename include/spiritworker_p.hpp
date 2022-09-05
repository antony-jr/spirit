#ifndef SPIRIT_WORKER_PRIVATE_HPP_INCLUDED
#define SPIRIT_WORKER_PRIVATE_HPP_INCLUDED
#include <QObject>
#include <QRect>
#include <QWidget>
#include <QHash>
#include <QBuffer>
#include <QTimer>
#include <QJsonObject>
#include <QScopedPointer>
#include <QArchive/QArchive>

class SpiritWorkerPrivate : public QObject {
	Q_OBJECT
public:
	SpiritWorkerPrivate();
	~SpiritWorkerPrivate();

public Q_SLOTS:
   	void getActions();
	void getCurrentAction();
	void getInfo();

	void init();
	void cancel();

	void setSpiritFile(const QString&);
	void setAction(const QString&);

private Q_SLOTS:
   	void handleProgress();
	void handleArchiveStarted();
	void handleArchiveCancel();
   	void handleArchiveError(short);
	void handleArchiveContents(QArchive::MemoryExtractorOutput*);

	/*
	 * Different Parsing Methods for 
	 * each Edition.
	 *
	 * Valid Editions,
	 *
	 * 1. 2021 (aka. v0.1.0)
	 */
	bool parseEdition2021(const QVector<QArchive::MemoryFile>&);

private:
	void clear(bool emitCanceled = true);

Q_SIGNALS:
	/// Gives the current webp to 
	/// to display in binary.
	/// Assume this binary is a webp file.
	void  action(QString /*current action name*/,
	      	     QBuffer* /*current webp file*/,
	      	     QBuffer* /*current webp file varaint*/,
		     QBuffer* /*audio file if given else nullptr*/,
	      	     bool /*loop*/,
		     int /*scale percentage*/,
		     int /*speed percentage*/,
		     QString /*next action if available*/,
		     QVector<int> /*offsets*/);

	void status(short);

	void initialized(QJsonObject);
	void started();
	void canceled();	
	void error(short);
	void actions(QList<QString>);
	void info(QJsonObject);
private:
	struct Action {
	   QString action;
	   QString next_action; /// Optional
	   QBuffer *buffer = nullptr; 
	   QBuffer *_buffer = nullptr;
	   QBuffer *play = nullptr;
	   bool loop = false;
	   QVector<int> offsets = {0, 0, 0, 0};
	   int speed = 100;
	   int scale = 100;
	};

private:
	QJsonObject m_Meta;
	short n_Status = 0; /// 0 => Idle (Always).
	bool b_CancelRequested = false;
	QString m_CurrentActionId;
	Action *m_CurrentAction = nullptr;

	/// All data required for a spirit is in the .spirit file which is a 
	/// ZIP file with a specific directory structure. 
	QString m_SpiritPath;
	QScopedPointer<QArchive::MemoryExtractorOutput> m_Spirit;
	QJsonObject meta;
	//// -----

	/// A internal state 
	/// to denote the current state of 
	/// the extractor.
	bool b_IsExtractorRunning = false;
	QScopedPointer<QArchive::MemoryExtractor> m_Extractor;


	/// Actions Hash Table
	/// ----
	/// Mapping to action string to the private
	/// Action structure which has buffer to the 
	/// the frames required and other crucial information. 
	QScopedPointer<QHash<QString, Action*>> m_Actions;
};

#endif // SPIRIT_WORKER_PRIVATE_HPP_INCLUDED
