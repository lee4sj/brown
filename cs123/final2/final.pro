# 
# CS123 Final Project Starter Code
# Adapted from starter code graciously provided by CS195-U: 3D Game Engines
#

QT += core gui opengl

TARGET = final
TEMPLATE = app

# If you add your own folders, add them to INCLUDEPATH and DEPENDPATH, e.g.
# INCLUDEPATH += folder1 folder2
# DEPENDPATH += folder1 folder2

SOURCES += main.cpp \
    mainwindow.cpp \
    view.cpp \
    FractalTree.cpp \
    math/CS123Vector.inl

HEADERS += mainwindow.h \
    view.h \
    FractalTree.h \
    math/CS123Algebra.h \
    lib/CS123Common.h

FORMS += mainwindow.ui
