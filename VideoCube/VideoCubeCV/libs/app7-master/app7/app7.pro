QT       += core gui widgets opengl xml

TARGET = app7
TEMPLATE = app
DEFINES+=_CRT_SECURE_NO_WARNINGS
INCLUDEPATH+="..//include"
INCLUDEPATH+="..//3PartyLibs"
MOC_DIR=.moc
OBJECTS_DIR=.o
#PRECOMPILED_HEADER = ../include/stdafx.h
QMAKE_CXXFLAGS+=-fpermissive
DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += c++11
SOURCES += main.cpp \
    trajectorydialog.cpp \
    animationdlg.cpp \
    biped.cpp \
    ref.cpp \
    ploter.cpp \
    selsetsdlg.cpp \
    mainwindow.cpp \
    terraindlg.cpp \
    linuxoid.cpp \
    genecurvefitter.cpp \
    animsprite.cpp


HEADERS +=  animationdlg.h \
    biped.h \
    ref.h \
#    selsetsdlg.h \
    trajectorydialog.h \
    selsetsdlg.h \
    mainwindow.h \
    geometrictests.h \
    ..\include\weapon.h \
    ..\include\terraindlg.h \
    linuxoid.h \
    animsprite.h \
    ..\include\amimpath.h


OTHER_FILES += \
    android\AndroidManifest.xml \
    android\res\drawable\icon.png \
    android\res\drawable\logo.png \
    android\res\drawable-hdpi\icon.png \
    android\res\drawable-ldpi\icon.png \
    android\res\drawable-mdpi\icon.png \
    android\res\layout\splash.xml \
    android\res\values\libs.xml \
    android\res\values\strings.xml \
    android\res\values-de\strings.xml \
    android\res\values-el\strings.xml \
    android\res\values-es\strings.xml \
    android\res\values-et\strings.xml \
    android\res\values-fa\strings.xml \
    android\res\values-fr\strings.xml \
    android\res\values-id\strings.xml \
    android\res\values-it\strings.xml \
    android\res\values-ja\strings.xml \
    android\res\values-ms\strings.xml \
    android\res\values-nb\strings.xml \
    android\res\values-nl\strings.xml \
    android\res\values-pl\strings.xml \
    android\res\values-pt-rBR\strings.xml \
    android\res\values-ro\strings.xml \
    android\res\values-rs\strings.xml \
    android\res\values-ru\strings.xml \
    android\res\values-zh-rCN\strings.xml \
    android\res\values-zh-rTW\strings.xml \
    android\src\org\kde\necessitas\ministro\IMinistro.aidl \
    android\src\org\kde\necessitas\ministro\IMinistroCallback.aidl \
    android\src\org\kde\necessitas\origo\QtActivity.java \
    android\src\org\kde\necessitas\origo\QtApplication.java \
    android\version.xml \
    release\objects.ini \
    release\app6.ini \
    debug\objects.ini \
    debug\xml_shemes\scene.xml \
    ..\Data\shaders\fshader.glsl \
    ..\Data\shaders\gshader.glsl \
    ..\Data\shaders\vshader.glsl \
    ..\Data\car.ini

FORMS += \
    mainwindow.ui \
    trajectorydialog.ui \
    selsetsdlg.ui \
    animationdlg.ui \
    terraindlg.ui


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../app7CoreLib/release/ -lapp7CoreLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../app7CoreLib/debug/ -lapp7CoreLib
else:unix: LIBS += -L$$OUT_PWD/../app7CoreLib/ -lapp7CoreLib

INCLUDEPATH += $$PWD/../app7CoreLib
DEPENDPATH += $$PWD/../app7CoreLib

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../app7CoreLib/release/libapp7CoreLib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../app7CoreLib/debug/libapp7CoreLib.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../app7CoreLib/release/app7CoreLib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../app7CoreLib/debug/app7CoreLib.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../app7CoreLib/libapp7CoreLib.a

#unix|win32: LIBS += -L$$PWD/../3PartyLibs/freeglut/lib/x64/ -lfreeglut_static
unix|win32: LIBS += -L$$PWD/../3PartyLibs/freeglut/lib/x64/ -lfreeglut.dll
INCLUDEPATH += $$PWD/../3PartyLibs/freeglut/include
DEPENDPATH += $$PWD/../3PartyLibs/freeglut/include


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../app7Dyn/release/ -lapp7Dyn
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../app7Dyn/debug/ -lapp7Dyn
else:unix: LIBS += -L$$OUT_PWD/../app7Dyn/ -lapp7Dyn

INCLUDEPATH += $$PWD/../app7Dyn
DEPENDPATH += $$PWD/../app7Dyn

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../app7Dyn/release/libapp7Dyn.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../app7Dyn/debug/libapp7Dyn.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../app7Dyn/release/app7Dyn.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../app7Dyn/debug/app7Dyn.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../app7Dyn/libapp7Dyn.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../app7ML/release/ -lapp7MLlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../app7ML/debug/ -lapp7MLlib
else:unix: LIBS += -L$$OUT_PWD/../app7ML/ -lapp7MLlib

INCLUDEPATH += $$PWD/../app7ML
DEPENDPATH += $$PWD/../app7ML

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../app7ML/release/libapp7MLlib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../app7ML/debug/libapp7MLlib.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../app7ML/release/app7MLlib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../app7ML/debug/app7MLlib.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../app7ML/libapp7MLlib.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../3PartyLibs/lib/ -lqwt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../3PartyLibs/lib/ -lqwtd
else:unix: LIBS += -L$$PWD/../3PartyLibs/lib/ -lqwt

INCLUDEPATH += $$PWD/../3PartyLibs/src
DEPENDPATH += $$PWD/../3PartyLibs/src

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../3PartyLibs/lib/libqwt.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../3PartyLibs/lib/libqwtd.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../3PartyLibs/lib/qwt.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../3PartyLibs/lib/qwtd.lib
else:unix: PRE_TARGETDEPS += $$PWD/../3PartyLibs/lib/libqwt.a



win32: LIBS += -L$$PWD/../../../../../../../Qt/Qt5.12.10/Tools/mingw730_64/x86_64-w64-mingw32/lib/ -lopengl32

INCLUDEPATH += $$PWD/../../../../../../../Qt/Qt5.12.10/Tools/mingw730_64/x86_64-w64-mingw32/include
DEPENDPATH += $$PWD/../../../../../../../Qt/Qt5.12.10/Tools/mingw730_64/x86_64-w64-mingw32/include


win32: LIBS += -lglu32
