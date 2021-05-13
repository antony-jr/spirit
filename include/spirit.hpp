#ifndef SPIRIT_HPP_INCLUDED
#define SPIRIT_HPP_INCLUDED
#include <QtWidgets/QLabel>

class Spirit : public QLabel {
	Q_OBJECT
public:
	Spirit();
	~Spirit();
public Q_SLOTS:
	void update(int, int, unsigned, unsigned);
};

#endif // SPIRIT_HPP_INCLUDED
