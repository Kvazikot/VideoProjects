QT += widgets opengl
DEFINES+=_CRT_SECURE_NO_WARNINGS
HEADERS =  mainwindow.h \
           planet.h
SOURCES        = main.cpp \
                 mainwindow.cpp

QMAKE_LFLAGS += /INCREMENTAL:NO
INCLUDEPATH+="..\..\..\app6\include"
INCLUDEPATH+="..\..\3PartyLibs\Include\wm5"

LIBS+="..\..\..\app6\3PartyLibs\glut32.lib"
LIBS+="..\..\..\app6\3PartyLibs\glaux.lib"

win32:Release{
# LIBS+="..\..\..\app6\app6Dyn\release\app6Dyn.lib"
 LIBS+="..\..\..\app6\app6CoreLib\release\app6CoreLib.lib"
 #LIBS+="plugins\pnp_basictools.lib"
 #LIBS+="..\..\..\app6\3PartyLibs\vs2010\qwt.lib"
 LIBS+="..\..\..\app6\3PartyLibs\vs2010\Wm5Mathematics.lib"
 LIBS+="..\..\..\app6\3PartyLibs\vs2010\Wm5Core.lib"
}
win32:Debug{
# LIBS+="..\..\..\app6\app6Dyn\debug\app6Dyn.lib"
 LIBS+="..\..\..\app6\app6CoreLib\debug\app6CoreLib.lib"
 #LIBS+="plugins\pnp_basictoolsd.lib"
 #LIBS+="..\..\3PartyLibs\vs2010\qwtd.lib"
 LIBS+="..\..\3PartyLibs\vs2010\Wm5MathematicsD.lib"
 LIBS+="..\..\3PartyLibs\vs2010\Wm5CoreD.lib"
}


# install
INSTALLS += target

SOURCES += \
    planets.cpp \
    planet.cpp
