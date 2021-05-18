INCLUDEPATH += $$PWD $$PWD/include
QT += core widgets network
CONFIG += static
LIBS += -lxdo 
TARGET = spirit
TEMPLATE = app
CONFIG += release

HEADERS += \
    $$PWD/include/termcolor.hpp \
    $$PWD/include/xdo_wrapper.hpp \
    $$PWD/include/windowinfo_p.hpp \
    $$PWD/include/windowinfo.hpp \
    $$PWD/include/helpers_p.hpp \
    $$PWD/include/spirit.hpp \
    $$PWD/include/bashrcwriter.hpp

SOURCES += \
    $$PWD/src/spirit.cc \
    $$PWD/src/windowinfo_p.cc \ 
    $$PWD/src/windowinfo.cc \
    $$PWD/src/helpers_p.cc \
    $$PWD/src/bashrcwriter.cc \
    $$PWD/src/main.cc 

RESOURCES = resources.qrc
