#ifndef SPIRIT_HPP_INCLUDED
#define SPIRIT_HPP_INCLUDED
#include <QtWidgets/QLabel>

// The spelling is intentional.
class Spirit : public QLabel {
	Q_OBJECT
	bool guess = true;
	bool debug = false;
	int xoff = 0,
	    yoff = 0;
	int w = 200,
	    h = 200;
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
