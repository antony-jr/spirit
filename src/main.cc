#include <QApplication>
#include "spiritapp.hpp"

int main(int ac, char **av) {
    QApplication app(ac, av);
    QApplication::setQuitOnLastWindowClosed(false);

    SpiritApp sapp;
    QObject::connect(&sapp, &SpiritApp::quit, &app, &QApplication::quit);

    sapp.run();
    return app.exec();
}
