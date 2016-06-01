#-------------------------------------------------
#
# Project created by QtCreator 2014-08-21T23:34:09
#
#-------------------------------------------------

QT +=  core gui widgets network webkitwidgets


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mywebview.cpp \
    settiing.cpp \
    his.cpp \
    bookmark.cpp

HEADERS  += mainwindow.h \
    mywebview.h \
    settiing.h \
    his.h \
    bookmark.h

FORMS    += mainwindow.ui \
    settiing.ui \
    his.ui \
    bookmark.ui

RESOURCES += \
    qrc.qrc
