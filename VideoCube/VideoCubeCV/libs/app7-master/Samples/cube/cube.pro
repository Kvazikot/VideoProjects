QT       += core gui widgets

TARGET = cube
TEMPLATE = app

SOURCES += main.cpp \
    terraindlg.cpp \
    mterrain.cpp \
    fractalterrain.cpp \
    mazegen.cpp \
    geometryobject.cpp


QT += opengl

SOURCES += mainwidget.cpp \
           geometryengine.cpp

HEADERS += \
        mainwidget.h \
        geometryengine.h \
        terraindlg.h \
        mterrain.h \
        fractalterrain.h \
    mazegen.h \
    geometryobject.h

INCLUDEPATH+="..\..\..\app6\include"
LIBS+="..\..\..\app6\3PartyLibs\glut32.lib"
LIBS+="..\..\..\app6\3PartyLibs\glaux.lib"

win32:Debug{
 LIBS+="..\..\..\app6\app6CoreLib\debug\app6CoreLib.lib"
}

RESOURCES += \
        textures.qrc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/opengl/cube
INSTALLS += target

FORMS += \
    terraindlg.ui

OTHER_FILES += \
    shaders/fshader.glsl \
    shaders/gshader.glsl \
    shaders/vshader.glsl
