#ifndef SPIRIT_HPP_INCLUDED
#define SPIRIT_HPP_INCLUDED
#include <QtWidgets/QLabel>

class Spirit : public QLabel {
	Q_OBJECT
public:
	Spirit();
	~Spirit();
public Q_SLOTS:
	void setGraphic(const QString&); 
	void setWidth(int);
	void setHeight(int);
	void update(int, int);

private:
	int w = 200,
	    h = 200;

};

#endif // SPIRIT_HPP_INCLUDED
