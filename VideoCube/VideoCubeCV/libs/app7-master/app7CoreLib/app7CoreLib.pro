#-------------------------------------------------
#
# Project created by QtCreator 2015-07-27T15:28:22
#
#-------------------------------------------------


QT       += gui core opengl xml

TARGET = app7CoreLib
TEMPLATE = lib
CONFIG += staticlib
DEFINES += APP7CORELIB_LIBRARY
DEFINES+=_CRT_SECURE_NO_WARNINGS
INCLUDEPATH+="..\include"
INCLUDEPATH+="/usr/include"
QMAKE_CXXFLAGS+=-fpermissive -std=c++11
#PRECOMPILED_HEADER = ..\include\stdafx.h

MOC_DIR=.moc
OBJECTS_DIR=.o
SOURCES += app6corelib.cpp \
    src/camera.cpp \
    cinput.cpp \
    src/cmaininterface.cpp \
    cobjregistry.cpp \
    curve.cpp \
    src/drawings2d.cpp \
    src/flags.cpp \
    gemobject.cpp \
    src/geometry.cpp \
    glcode.cpp \
    src/grid.cpp \
    src/group.cpp \
    src/interp.cpp \
    src/kirpich.cpp \
    src/matrix.cpp \
    src/mesh.cpp \
    src/move.cpp \
    src/object.cpp \
    src/param_screen_bcb.cpp \
    src/print.cpp \
    src/random_staff.cpp \
    src/renderobject.cpp \
    selection.cpp \
    src/shared_data.cpp \
    src/str_utils.cpp \
    src/TGATexture.cpp \
    src/timer.cpp \
    var_parse.cpp \
    src/vbuf.cpp \
    src/vector3d.cpp \
    src/vpnt.cpp \
    src/stb_image.c \
    src/animation.cpp \
#    src/muapi.cpp \
    material.cpp \
    src/ball.cpp \
    src/glwidget.cpp \
#    src/wglwidget.cpp \
    src/osd.cpp \
    src/tranform.cpp \
    src/window.cpp \
    glwidget2.cpp \
    objectinspector.cpp \
#    sky.cpp \
    geometryobject.cpp \
    maxcurve.cpp \
    rectangle.cpp

HEADERS += app6corelib.h\
        app6corelib_global.h \
    ../include/animation.h \
    ../include/ascii3d.h \
    ../include/objectinspector.h \
    ../include/assoc_map.h \
    ../include/autopilot.h \
    ../include/camera.h \
    ../include/cinput.h \
    ../include/cmaininterface.h \
    ../include/cobjregistry.h \
    ../include/curve.h \
    ../include/demper.H \
    ../include/drawings2d.h \
    ../include/flags.h \
    ../include/gemobject.h \
    ../include/geometry.h \
    ../include/glcode.h \
    ../include/glwidget.h \
    ../include/glwidget2.h \
    ../include/glwindow.h \
    ../include/grid.h \
    ../include/group.h \
    ../include/interp.h \
    ../include/kirpich.h \
    ../include/law.h \
    ../include/material.h \
    ../include/matrix.h \
    ../include/mesh.h \
    ../include/move.h \
    ../include/muapi.h \
    ../include/object.h \
    ../include/param_screen_bcb.h \
    ../include/plane.h \
    ../include/planesim.h \
    ../include/ploter.h \
    ../include/polet.h \
    ../include/print.h \
    ../include/quaterion.h \
    ../include/random_staff.h \
    ../include/renderobject.h \
    ../include/selection.h \
    ../include/shared_data.h \
    ../include/stb_image.h \
    ../include/str_utils.h \
    ../include/terrain.h \
    ../include/terrain_params.h \
    ../include/TGATexture.H \
    ../include/timer.h \
    ../include/traj_gen.h \
    ../include/trajectorydialog.h \
    ../include/ui_trajectorydialog.h \
    ../include/var_parse.h \
    ../include/vbuf.h \
    ../include/vector3d.h \
    ../include/vpnt.h \
    ../include/player.h \
    ../include/ball.h \
    ../include/window.h \
    ../include/sky.h \
    ../include/geometryobject.h \
     ../include/maxcurve.h \
    rectangle.h \
    ../include/rectangle.h

win32: LIBS += -L$$PWD/../3PartyLibs/freeglut/lib/x64 -lfreeglut.dll

INCLUDEPATH += $$PWD/../3PartyLibs/freeglut/include
DEPENDPATH += $$PWD/../3PartyLibs/freeglut/include

unix {
    target.path = /usr/lib
    INSTALLS += target
}
