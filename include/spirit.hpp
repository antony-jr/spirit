#ifndef SPIRIT_HPP_INCLUDED
#define SPIRIT_HPP_INCLUDED
#include <QtWidgets/QLabel>

// The spelling is intentional.
class Spirit : public QLabel {
	Q_OBJECT
public:
	Spirit(const QString&/*filename*/, bool is_png = false);
	~Spirit();
public Q_SLOTS:
	void update(int, int, unsigned, unsigned);
Q_SIGNALS:
	void error(QString);
};

#endif // SPIRIT_HPP_INCLUDED
