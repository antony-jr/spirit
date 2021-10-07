#ifndef SPIRIT_WORKER_PRIVATE_HPP_INCLUDED
#define SPIRIT_WORKER_PRIVATE_HPP_INCLUDED
#include <QRect>
#include <QWidget>
#include <QHash>
#include <QMutex>
#include <QScopedPointer>
#include <QArchive/QArchive>

class SpiritWorkerPrivate : public QObject {
	Q_OBJECT
public:
	SpiritWorkerPrivate(QObject *parent = nullptr);
	~SpiritWorkerPrivate();

public Q_SLOTS:
   	void getActions();
	void getCurrentAction();
	void getInfo();

   	void start();
	void cancel();

	void setSpiritFile(const QString&);
	void setAction(const QString&);

private Q_SLOTS:
   	void handleProgress();
	void handleArchiveCancel();
   	void handleArchiveError(short);
	void handleArchiveContents(QArchive::MemoryExtractorOutput*);

	/// Animation Loop
	void animationLoop();


	/*
	 * Different Parsing Methods for 
	 * each Edition.
	 *
	 * Valid Editions,
	 *
	 * 1. 2021 (aka. v0.1.0)
	 */
	bool parseEdition2021(QArchive::MemoryExtractorOutput*);

private:
	void clear(bool emitCanceled = true);

Q_SIGNALS:
	/// Gives the current frame to 
	/// to display in binary.
	/// Assume this binary is a image file.
	void frame(QBuffer*);

	void status(short);

	/// Emitted with basic info about rendering
	/// the spirit like y offset.
	void started(QJsonObject);
	void canceled();	
	void error(short);
	void actions(QList<QString>);
	void action(QString);
	void actionChanged(QString);
	void info(QJsonObject);
private:
	struct Action {
	   QString action;
	   QString next_action; /// Optional 
	   QVector<QBuffer*> frames;
	   QVector<QPair<int, int>> frame_order;
	   int interval = 500; // Interval in msecs.
	   QBuffer *play = nullptr;
	   bool loop = false;
	};

private:
	QJsonObject m_Meta;
	short n_Status = 0; /// 0 => Idle (Always).
	bool b_CancelRequested = false;
	QString m_CurrentAction;

	/// Animation State
	/// ---
	int n_FrameUnit = 0; // Which frame order to use.
	int n_Frame = 0; // Which frame in the frame range 
	QScopedPointer<QTimer> m_AnimationTimer;

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
