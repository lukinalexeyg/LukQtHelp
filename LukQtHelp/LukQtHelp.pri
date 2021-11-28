INCLUDEPATH += $$PWD

DEPENDPATH += $$PWD

QT += core gui help sql

SOURCES += \
    $$PWD/lhelpengine.cpp \
    $$PWD/lhelptextbrowser.cpp \
    $$PWD/lhelptoolbar.cpp \
    $$PWD/lhelpviewer.cpp \
    $$PWD/lhelpwindow.cpp \
    $$PWD/lstandalonemainwindow.cpp

HEADERS += \
    $$PWD/lhelpengine.h \
    $$PWD/lhelptextbrowser.h \
    $$PWD/lhelptoolbar.h \
    $$PWD/lhelpviewer.h \
    $$PWD/lhelpwindow.h \
    $$PWD/lstandalonemainwindow.h

RESOURCES += \
    $$PWD/resources.qrc
