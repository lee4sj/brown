#-------------------------------------------------
#
# Project created by QtCreator 2012-12-11T17:25:34
#
#-------------------------------------------------

QT       += core gui

TARGET = test
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ../tree.cc \
    ../props.cc \
    ../pongish.cc \
    ../gltree.cc \
    ../circle.cc \
    ../animate.cc

HEADERS  += mainwindow.h \
    ../props.hh \
    ../gltree.hh \
    ../circle.hh

FORMS    += mainwindow.ui

OTHER_FILES += \
    ../weirdo.dna \
    ../tree-stringy.dna \
    ../tree-4.dna \
    ../tree-3.dna \
    ../tree-2.dna \
    ../tree-1.dna \
    ../readme.pdf \
    ../README \
    ../noanimate.dna \
    ../makefile \
    ../animate-long.dna \
    ../animate-long-fern.dna
