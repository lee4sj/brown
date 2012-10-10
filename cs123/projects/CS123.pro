QT += opengl \
    xml
TARGET = CS123
TEMPLATE = app
SOURCES += brush/SmudgeBrush.cpp \
    brush/QuadraticBrush.cpp \
    brush/LinearBrush.cpp \
    brush/ConstantBrush.cpp \
    brush/Brush.cpp \
    camera/OrbitingCamera.cpp \
    camera/CamtransCamera.cpp \
    scenegraph/SelectionRecorder.cpp \
    lib/CS123XmlSceneParser.cpp \
    math/CS123Matrix.cpp \
    scenegraph/ShapesScene.cpp \
    scenegraph/SceneviewScene.cpp \
    scenegraph/Scene.cpp \
    scenegraph/RayScene.cpp \
    scenegraph/OpenGLScene.cpp \
    ui/SupportCanvas3D.cpp \
    ui/SupportCanvas2D.cpp \
    ui/Settings.cpp \
    ui/mainwindow.cpp \
    ui/Databinding.cpp \
    ui/Canvas3D.cpp \
    ui/Canvas2D.cpp \
    main.cpp \
    scenegraph/shapes/Cube.cpp \
    scenegraph/shapes/Shapes.cpp \
    scenegraph/shapes/Cylinder.cpp \
    scenegraph/shapes/Cone.cpp \
    scenegraph/shapes/Sphere.cpp \
    filter/InvertFilter.cpp \
    filter/Filter.cpp \
    filter/GreyscaleFilter.cpp \
    filter/EdgeDetectFilter.cpp \
    filter/BlurFilter.cpp
HEADERS += brush/SmudgeBrush.h \
    brush/QuadraticBrush.h \
    brush/LinearBrush.h \
    brush/ConstantBrush.h \
    brush/Brush.h \
    camera/OrbitingCamera.h \
    camera/CamtransCamera.h \
    camera/Camera.h \
    scenegraph/SelectionRecorder.h \
    lib/CS123XmlSceneParser.h \
    lib/CS123SceneData.h \
    lib/CS123ISceneParser.h \
    lib/CS123Common.h \
    math/CS123Algebra.h \
    math/CS123Matrix.inl \
    math/CS123Vector.inl \
    math/TA_Tester.h \
    scenegraph/ShapesScene.h \
    scenegraph/SceneviewScene.h \
    scenegraph/Scene.h \
    scenegraph/RayScene.h \
    scenegraph/OpenGLScene.h \
    ui/SupportCanvas3D.h \
    ui/SupportCanvas2D.h \
    ui/Settings.h \
    ui/mainwindow.h \
    ui/Databinding.h \
    ui/Canvas3D.h \
    ui/Canvas2D.h \
    scenegraph/shapes/Cube.h \
    scenegraph/shapes/Shapes.h \
    scenegraph/shapes/Cylinder.h \
    scenegraph/shapes/Cone.h \
    scenegraph/shapes/Sphere.h \
    filter/Filter.h \
    filter/InvertFilter.h \
    filter/GreyscaleFilter.h \
    filter/EdgeDetectFilter.h \
    filter/BlurFilter.h
FORMS += ui/mainwindow.ui
INCLUDEPATH += brush \
    camera \
    lib \
    math \
    scenegraph \
    ui
DEPENDPATH += brush \
    camera \
    lib \
    math \
    scenegraph \
    ui
DEFINES += TIXML_USE_STL
OTHER_FILES += 

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
macx:QMAKE_CXXFLAGS_WARN_ON -= -Warray-bounds \
    -Wc++0x-compat
