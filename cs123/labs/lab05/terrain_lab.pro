#-------------------------------------------------
#
# Project created by QtCreator 2011-07-31T12:33:32
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = terrain_lab
TEMPLATE = app

INCLUDEPATH += src
DEPENDPATH += src

SOURCES += src/mainwindow.cpp \
    src/main.cpp \
    src/canvas.cpp \
    src/vector.inl

HEADERS  += src/vector.h \
    src/mainwindow.h \
    src/labcamera.h \
    src/canvas.h

FORMS    += src/mainwindow.ui

# Don't add the -pg flag unless you know what you are doing. It makes QThreadPool freeze on Mac OS X
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3
QMAKE_CXXFLAGS_WARN_ON -= -Wall
QMAKE_CXXFLAGS_WARN_ON += -Waddress \
    -Warray-bounds \
    -Wc++0x-compat \
    -Wchar-subscripts \
    -Wformat \
    -Wmain \
    -Wmissing-braces \
    -Wparentheses \
    -Wreorder \
    -Wreturn-type \
    -Wsequence-point \
    -Wsign-compare \
    -Wstrict-aliasing \
    -Wstrict-overflow=1 \
    -Wswitch \
    -Wtrigraphs \
    -Wuninitialized \
    -Wunused-label \
    -Wunused-variable \
    -Wvolatile-register-var \
    -Wno-extra

# QMAKE_CXX_FLAGS_WARN_ON += -Wunknown-pragmas -Wunused-function -Wmain
macx:QMAKE_CXXFLAGS_WARN_ON -= -Warray-bounds -Wc++0x-compat

RESOURCES +=
