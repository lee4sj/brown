#-------------------------------------------------
#
# Project created by QtCreator 2012-07-22T19:04:51
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = Pong
TEMPLATE = app

INCLUDEPATH += src
DEPENDPATH += src

SOURCES += src/main.cpp\
           src/mainwindow.cpp \
           src/glwidget.cpp \
           src/pong.cpp

HEADERS  += src/mainwindow.h \
            src/glwidget.h \
            src/pong.h

FORMS    += src/mainwindow.ui

OTHER_FILES +=
