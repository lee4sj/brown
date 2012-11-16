QT += core gui opengl

TARGET = opengl2_lab
TEMPLATE = app

INCLUDEPATH += src
DEPENDPATH += src

SOURCES += src/settings.cpp \
    src/mainwindow.cpp \
    src/main.cpp \
    src/glwidget.cpp \
    src/databinding.cpp

HEADERS += src/vector.h \
    src/settings.h \
    src/mainwindow.h \
    src/glwidget.h \
    src/databinding.h \
    src/camera.h

FORMS += src/mainwindow.ui

OTHER_FILES +=
