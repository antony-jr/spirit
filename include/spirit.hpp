#ifndef SPIRIT_HPP_INCLUDED
#define SPIRIT_HPP_INCLUDED
#include <QtWidgets/QLabel>

// The spelling is intentional.
class Spirit : public QLabel {
	Q_OBJECT
public:
	enum HAlign { 
		Center,
		Left,
		Right
	};
	Spirit();
	~Spirit();
public Q_SLOTS:
	void onTop();
	void setHorizontalAlignment(HAlign);
	void setDebug(bool);
	void setXOffset(int);
	void setYOffset(int);
	void setWidth(int);
	void setHeight(int);
	void setGraphic(const QString&, bool);
	void update(int, int, unsigned, unsigned);

private:
	HAlign align = HAlign::Left;
	bool guess = true;
	bool debug = false;
	int xoff = 0,
	    yoff = 0;
	int w = 200,
	    h = 200;

};

#endif // SPIRIT_HPP_INCLUDED
