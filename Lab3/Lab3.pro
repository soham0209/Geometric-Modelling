#-------------------------------------------------
#
# Project created by QtCreator 2018-03-29T17:58:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Lab3
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        window.cpp \
    handler.cpp \
    BezierSurface.cpp \
    BsplineSurface.cpp \
    CatmullClark.cpp \
    DooSabin.cpp \
    Loop.cpp \
    mesh.cpp \
    Utils.cpp \
    BezierSurface.cpp \
    BsplineSurface.cpp \
    CatmullClark.cpp \
    DooSabin.cpp \
    handler.cpp \
    Loop.cpp \
    main.cpp \
    mesh.cpp \
    Utils.cpp \
    window.cpp

HEADERS += \
        window.h \
    BezierSurface.h \
    BsplineSurface.h \
    CatmullClark.h \
    DooSabin.h \
    geometry.h \
    Loop.h \
    mesh.h \
    Utils.h \
    window.h \
    handler.h \
    BezierSurface.h \
    BsplineSurface.h \
    CatmullClark.h \
    DooSabin.h \
    geometry.h \
    handler.h \
    Loop.h \
    mesh.h \
    Utils.h \
    window.h

DISTFILES += \
    input/a.off \
    input/asu.off \
    input/asu_tri.off \
    input/asucc1.off \
    input/asucc2.off \
    input/asucc3.off \
    input/asuds1.off \
    input/asuds2.off \
    input/asuds3.off \
    input/asuloop1.off \
    input/asuloop2.off \
    input/asuloop3.off \
    input/boxcube.off \
    input/boxtorus.off \
    input/concave.off \
    input/cone.off \
    input/cross.off \
    input/cube.off \
    input/cubepoints.off \
    input/cuboctahedron.off \
    input/dodecahedron.off \
    input/epcot.off \
    input/extrusion.off \
    input/extrusion2.off \
    input/extrusionpoints.off \
    input/extrusionpoints2.off \
    input/helix2.off \
    input/hole.off \
    input/icosahedron.off \
    input/icosidodecahedron.off \
    input/lofting.off \
    input/loopNet.off \
    input/octahedron.off \
    input/octtorus.off \
    input/parabola.off \
    input/parabolapoints.off \
    input/revolution.off \
    input/revolutionpoints.off \
    input/rhombicosidodecahedron.off \
    input/rhombicuboctahedron.off \
    input/rhombitruncated_cubeoctahedron.off \
    input/rhombitruncated_icosidodecahedron.off \
    input/s.off \
    input/snub_cube.off \
    input/snub_dodecahedron.off \
    input/sphere.off \
    input/sphere2.off \
    input/tet3d.off \
    input/tetrahedron.off \
    input/torus.off \
    input/triangle.off \
    input/truncated_cube.off \
    input/truncated_dodecahedron.off \
    input/truncated_icosahedron.off \
    input/truncated_octahedron.off \
    input/truncated_tetrahedron.off \
    input/try.off \
    input/u.off
