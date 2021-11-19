INCLUDEPATH += $$PWD

DEPENDPATH += $$PWD

QT += core gui help sql

SOURCES += \
    $$PWD/helptextbrowser.cpp \
    $$PWD/helpwindow.cpp \
    $$PWD/helpviewer.cpp \
    $$PWD/independentwindow.cpp

HEADERS  += \
    $$PWD/helptextbrowser.h \
    $$PWD/helpwindow.h \
    $$PWD/helpviewer.h \
    $$PWD/independentwindow.h

RESOURCES += \
    $$PWD/resources.qrc
