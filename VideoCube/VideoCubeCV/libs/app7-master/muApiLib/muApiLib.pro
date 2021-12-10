#-------------------------------------------------
#
# Project created by QtCreator 2017-03-02T14:17:50
#
#-------------------------------------------------

QT       -= gui

TARGET = muApiLib
TEMPLATE = lib
CONFIG += staticlib

SOURCES += muapilib.cpp \
    muapi_dll.cpp

HEADERS += muapilib.h \
    muapi_dll.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
