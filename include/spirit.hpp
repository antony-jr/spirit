#ifndef SPIRIT_HPP_INCLUDED
#define SPIRIT_HPP_INCLUDED
#include <QRect>
#include <QWidget>
#include <QHash>
#include <QScopedPointer>
#include <QArchive/QArchive>

#include "spiritenums.hpp"

class Spirit : public QWidget {
	Q_OBJECT
public:
	struct Error : SpiritEnums::Spirit::Error { };

	Spirit(const QString&);
	~Spirit();

public Q_SLOTS:
	void init();
	void update(QRect);

private Q_SLOTS:
	void doInit();
	void handleArchiveError(short);
	void handleArchiveContents(QArchive::MemoryExtractorOutput*);

Q_SIGNALS:
	void initialized();
	void error(short);
private:
	struct Action {
	   QString action;
	   QVector<QBuffer*> frames;
	   QVector<QPair<int, int>> frame_order;
	   QBuffer *play = nullptr;
	   bool loop = false;
	};
private:
	/// All data required for a spirit is in the .spirit file which is a 
	/// ZIP file with a specific directory structure. 
	QString m_SpiritPath;
	QScopedPointer<QArchive::MemoryExtractorOutput> m_Spirit;
	QJsonObject meta;

	QScopedPointer<QArchive::MemoryExtractor> m_Extractor;


	/// Actions 
	QScopedPointer<QHash<QString, Action*>> m_Actions;
};

#endif // SPIRIT_HPP_INCLUDED
