#-------------------------------------------------
#
# Project created by QtCreator 2017-08-09T20:41:36
#
#-------------------------------------------------

QT       += core gui
QMAKE_CXXFLAGS+=-fpermissive -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = app7MLlib
CONFIG += staticlib
MOC_DIR=.moc
OBJECTS_DIR=.o
TEMPLATE = lib

SOURCES +=  qlearning.cpp \
	    netbp.cpp \
	    transfer_fun.cpp \
    gradientdescent.cpp

HEADERS  += qlearning.h \
	    netbp.h \
	    transfer_fun.h \
    gradientdescent.h
