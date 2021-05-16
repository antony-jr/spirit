#ifndef SPIRIT_HPP_INCLUDED
#define SPIRIT_HPP_INCLUDED
#include <QtWidgets/QLabel>

// The spelling is intentional.
class Spirit : public QLabel {
	Q_OBJECT
	bool debug = false;
	int xoff = 90,
	    yoff = 200;
	int w = 240,
	    h = 240;
public:
	Spirit();
	~Spirit();
public Q_SLOTS:
	void onTop();
	void setDebug(bool);
	void setXOffset(int);
	void setYOffset(int);
	void setWidth(int);
	void setHeight(int);
	void setGraphic(const QString&, bool);
	void update(int, int, unsigned, unsigned);
};

#endif // SPIRIT_HPP_INCLUDED
