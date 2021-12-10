#-------------------------------------------------
#
# Project created by QtCreator 2017-08-09T20:41:36
#
#-------------------------------------------------

QT       += core gui
QMAKE_CXXFLAGS+=/std:c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = reinf
TEMPLATE = app

INCLUDEPATH+="..\3PartyLibs\Include\qwt"

win32:Debug{
    LIBS+="..\3PartyLibs\vs2012\qwtd.lib"
}

SOURCES += main.cpp\
           mainwindow.cpp \
    mingamewidget.cpp \
    qlearning.cpp \
    trash.cpp \
    netbp.cpp \
    nnwidget.cpp \
    test.cpp \
    transfer_fun.cpp \
    gradientdescent.cpp
#    qcustomplot.cpp

HEADERS  += mainwindow.h \
    mingamewidget.h \
    qlearning.h \
    trash.h \
    netbp.h \
    nnwidget.h \
    test.h \
    transfer_fun.h \
    gradientdescent.h
#            qcustomplot.h

FORMS    += mainwindow.ui
