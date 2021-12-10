QT += gui opengl

TEMPLATE = lib
DEFINES += GODBLESSENGINE_LIBRARY
QMAKE_CXXFLAGS += _CRT_SECURE_NOWARNINGS
CONFIG += c++11
INCLUDEPATH += "C:/Program Files (x86)/Windows Kits/10/Include/10.0.22000.0/ucrt"
#LIBS += -L"c:\Program Files (x86)\Windows Kits\10\Lib\10.0.22000.0\um\x64"
#-lGlU32 -lglut32 -lOpenGL32

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    TGATexture.cpp \
    godblessengine.cpp \
    stb_image.c

HEADERS += \
    GodBlessEngine_global.h \
    TGATexture.h \
    godblessengine.h \
    stb_image.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
