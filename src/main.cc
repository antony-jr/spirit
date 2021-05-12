#include <QApplication>
#include <QtWidgets/QLabel>

int main(int ac, char **av) {
	QApplication app(ac, av);
	QApplication::setQuitOnLastWindowClosed(false);

	QLabel label("Hello World!", nullptr, Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
	label.setAttribute(Qt::WA_TranslucentBackground);
        label.setStyleSheet(QString::fromUtf8("background: rgba(0,0,0,1); color: white;"));
        label.show();
	label.resize(30, 30);

	return app.exec();
}
