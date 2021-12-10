#-------------------------------------------------
#
# Project created by QtCreator 2015-07-27T22:11:07
#
#-------------------------------------------------

QT       += opengl

TARGET = app7Dyn
TEMPLATE = lib
CONFIG += staticlib
DEFINES += APP7DYN_LIBRARY
DEFINES+=_CRT_SECURE_NO_WARNINGS
INCLUDEPATH+="..\include"
INCLUDEPATH+="/usr/include"
MOC_DIR=.moc
OBJECTS_DIR=.o
QMAKE_CXXFLAGS+=-fpermissive -std=c++11

win32
{
  INCLUDEPATH+="..\3PartyLibs\Include\qwt"
  INCLUDEPATH+="..\3PartyLibs\Include\wm5"
  PRECOMPILED_HEADER = ..\include\stdafx.h
  LIBS+="..\3PartyLibs\glut32.lib"
  LIBS+="..\3PartyLibs\glaux.lib"
}

#win32:Release{
#  LIBS+="..\app6CoreLib\release\app6CoreLib.lib"
#}
#win32:Debug{
#  LIBS+="..\app6CoreLib\debug\app6CoreLib.lib"
#}

#LIBS+=-L"../app7CoreLib/" -lapp7CoreLib

SOURCES += demper.cpp \
    law.cpp \
    plane.cpp \
    planesim.cpp \
    polet.cpp \
    traj_gen.cpp \
    terrain.cpp \
    vehicle.cpp \
 #   graviball.cpp \
    player.cpp \
    spherevehiclecolision.cpp \
    mterrain.cpp \
    car_autopilot.cpp \
    ../Samples/HMProcessing/hmprocessing.cpp \
    mazegen.cpp \
    fractalterrain.cpp

HEADERS += app6dyn.h\
        app6dyn_global.h \
    ../include/vehicle.h \
    ../include/autopilot.h \
    ../include/demper.H \
    ../include/law.h \
    ../include/plane.h \
    ../include/planesim.h \
    ../include/polet.h \
    ../include/traj_gen.h \
    ../include/graviball.h \
    ../include/spherevehiclecolision.h \
    ../include/mterrain.h \
    car_autopilot.h \
    ../include/car_autopilot.h \
    ../Samples/HMProcessing/hmprocessing.h \
    ../include/mazegen.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32: LIBS += -L$$PWD/../3PartyLibs/freeglut/lib/ -lfreeglut

INCLUDEPATH += $$PWD/../3PartyLibs/freeglut/include
DEPENDPATH += $$PWD/../3PartyLibs/freeglut/include
