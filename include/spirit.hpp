#ifndef SPIRIT_HPP_INCLUDED
#define SPIRIT_HPP_INCLUDED
#include <QRect>
#include <QWidget>
#include <QScopedPointer>
#include <QArchive/QArchive>

#include "spiritenums.hpp"

class Spirit : public QWidget {
	Q_OBJECT
public:
	struct Error : SpiritEnums::Spirit::Error { };

	Spirit();
	Spirit(const QString&);

	void setPath(const QString&);
	void clear();

	void init();

	~Spirit();
public Q_SLOTS:
	void update(QRect);

private Q_SLOTS:
	void handleArchvieError(short);
	void handleArchiveContents(QArchive::MemoryExtractorOutput*);

Q_SIGNALS:
	void initialized();
	void error(short);
private:
	/// All data required for a spirit is in the .spirit file which is a 
	/// ZIP file with a specific directory structure. 
	QString m_SpiritPath;
	QScopedPointer<QArchive::MemoryExtractorOutput> m_Spirit;
	QJsonObject meta;
};

#endif // SPIRIT_HPP_INCLUDED
