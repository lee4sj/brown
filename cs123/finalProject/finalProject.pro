#-------------------------------------------------
#
# Project created by QtCreator 2012-12-16T15:04:53
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = finalProject
TEMPLATE = app

INCLUDEPATH += final \
    lib \
    math \
    support
DEPENDPATH += final \
    lib \
    math \
    support


SOURCES += \
    final/glwidget.cpp \
    lib/targa.cpp \
    lib/glm.cpp \
    support/resourceloader.cpp \
    support/mainwindow.cpp \
    support/main.cpp \
    support/camera.cpp \
    final/FractalTree.cpp \
    final/BranchCylinder.cpp \
    final/terrain.cpp

HEADERS  += \
    final/glwidget.h \
    lib/targa.h \
    lib/glm.h \
    math/vector.h \
    support/resourceloader.h \
    support/mainwindow.h \
    support/camera.h \
    final/FractalTree.h \
    final/BranchCylinder.h \
    lib/CS123SceneData.h \
    final/terrain.h

FORMS += \
    support/mainwindow.ui

OTHER_FILES += \
    final/leaf.vert \
    final/leaf.vars \
    final/leaf.frag \
    final/bump.vert \
    final/bump.frag
