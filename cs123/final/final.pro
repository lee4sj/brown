#-------------------------------------------------
#
# Project created by QtCreator 2012-12-13T21:12:17
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = final
TEMPLATE = app


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
    math/CS123Matrix.inl \
    math/CS123Matrix.cpp \
    math/CS123Vector.inl

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
    math/CS123Algebra.h \
    lib/CS123Common.h \
    lib/CS123SceneData.h

OTHER_FILES += \
    shaders/refract.vert \
    shaders/refract.frag \
    shaders/reflect.vert \
    shaders/reflect.frag \
    shaders/brightpass.frag \
    shaders/blur.vert \
    shaders/blur.vars \
    shaders/blur.frag \
    shaders/bump.vert \
    shaders/bump.frag

FORMS += \
    support/mainwindow.ui
