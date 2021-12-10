QT += widgets core concurrent opengl xml
OUTPUT += console
CONFIG += c++11
OPENCV_451_PATH = "d:\\opencv"
INCLUDEPATH += $$(PYTHON_PATH)\\include
#INCLUDEPATH +=$$OPENCV_451_PATH\\dlib\\dlib\\external\\pybind11\\include\\
INCLUDEPATH+="$$OPENCV_451_PATH\\modules\\core\\include"
INCLUDEPATH+="$$OPENCV_451_PATH\\build2\\modules"
INCLUDEPATH+="$$OPENCV_451_PATH\\build2\\"
INCLUDEPATH+="$$OPENCV_451_PATH\\include\\opencv2"
INCLUDEPATH+="$$OPENCV_451_PATH\\include\\"
INCLUDEPATH+="$$OPENCV_451_PATH\\modules\\calib3d\\include\\"
INCLUDEPATH+="$$OPENCV_451_PATH\\modules\\calib3d\\include"
INCLUDEPATH+="$$OPENCV_451_PATH\\modules\\core\\include"
INCLUDEPATH+="$$OPENCV_451_PATH\\modules\\dnn\\include"
INCLUDEPATH+="$$OPENCV_451_PATH\\modules\\features2d\\include"
INCLUDEPATH+="$$OPENCV_451_PATH\\modules\\flann\\include"
#INCLUDEPATH+="$$OPENCV_451_PATH\\modules\\gapi\\include"
INCLUDEPATH+="$$OPENCV_451_PATH\\modules\\highgui\\include"
INCLUDEPATH+="$$OPENCV_451_PATH\\modules\\imgcodecs\\include"
INCLUDEPATH+="$$OPENCV_451_PATH\\modules\\imgproc\\include"
INCLUDEPATH+="$$OPENCV_451_PATH\\modules\\videoio\\include"
INCLUDEPATH+="$$OPENCV_451_PATH\\modules\\video\\include"
INCLUDEPATH+="$$OPENCV_451_PATH\\modules\\gapi\\include"
INCLUDEPATH+="$$OPENCV_451_PATH\\modules\\objdetect\\include"
INCLUDEPATH+="$$OPENCV_451_PATH\\modules\photo\\include"
#INCLUDEPATH+="$$OPENCV_451_PATH\\modules\\java\\include"
#INCLUDEPATH+="$$OPENCV_451_PATH\\modules\\js\\include"
INCLUDEPATH+="$$OPENCV_451_PATH\\modules\\ml\\include"
#INCLUDEPATH+="$$OPENCV_451_PATH\\modules\\objc\\include"
#INCLUDEPATH+="$$OPENCV_451_PATH\\modules\\objdetect\\include"
#INCLUDEPATH+="$$OPENCV_451_PATH\\modules\\photo\\include"
#INCLUDEPATH+="$$OPENCV_451_PATH\\modules\\python\\include"
INCLUDEPATH+="$$OPENCV_451_PATH\\modules\\stitching\\include"
#INCLUDEPATH+="$$OPENCV_451_PATH\\modules\\ts\\include"
#INCLUDEPATH+="$$OPENCV_451_PATH\\modules\\video\\include"
#INCLUDEPATH+="$$OPENCV_451_PATH\\modules\\videoio\\include"
#INCLUDEPATH+="$$OPENCV_451_PATH\\modules\\world\\include"


win32:CONFIG(release, debug|release): LIBS+=-L"$$OPENCV_451_PATH\\build_mingw\\lib\\" -lopencv_core453
win32:CONFIG(release, debug|release): LIBS+=-L"$$OPENCV_451_PATH\\build_mingw\\lib\\" -lopencv_ml453
win32:CONFIG(release, debug|release): LIBS+=-L"$$OPENCV_451_PATH\\build_mingw\\lib\\" -lopencv_flann453
win32:CONFIG(release, debug|release): LIBS+=-L"$$OPENCV_451_PATH\\build_mingw\\lib\\" -lopencv_imgproc453
win32:CONFIG(release, debug|release): LIBS+=-L"$$OPENCV_451_PATH\\build_mingw\\lib\\" -lopencv_photo453
win32:CONFIG(release, debug|release): LIBS+=-L"$$OPENCV_451_PATH\\build_mingw\\lib\\" -lopencv_features2d453
win32:CONFIG(release, debug|release): LIBS+=-L"$$OPENCV_451_PATH\\build_mingw\\lib\\" -lopencv_imgcodecs453
LIBS+=-L"$$OPENCV_451_PATH\\build_mingw\\lib\\" -lopencv_videoio453
LIBS+=-L"$$OPENCV_451_PATH\\build_mingw\\lib\\" -lopencv_highgui453
win32:CONFIG(release, debug|release): LIBS+=-L"$$OPENCV_451_PATH\\build_mingw\\lib\\" -lopencv_ts453
win32:CONFIG(release, debug|release): LIBS+=-L"$$OPENCV_451_PATH\\build_mingw\\lib\\" -lopencv_calib3d453
win32:CONFIG(release, debug|release): LIBS+=-L"$$OPENCV_451_PATH\\build_mingw\\lib\\" -lopencv_objdetect453
win32:CONFIG(release, debug|release): LIBS+=-L"$$OPENCV_451_PATH\\build_mingw\\lib\\" -lopencv_stitching453
win32:CONFIG(release, debug|release): LIBS+=-L"$$OPENCV_451_PATH\\build_mingw\\lib\\" -lopencv_dnn453
win32:CONFIG(release, debug|release): LIBS+=-L"$$OPENCV_451_PATH\\build_mingw\\lib\\" -lopencv_video453
win32:CONFIG(release, debug|release): LIBS+=-L"$$(PYTHON_PATH)\\libs\\" -lpython39


#win32:CONFIG(debug, debug|release): LIBS+=-L"$$OPENCV_451_PATH\\build_mingw\\lib\\" -lopencv_core453d
#win32:CONFIG(debug, debug|release): LIBS+=-L"$$OPENCV_451_PATH\\build_mingw\\lib\\" -lopencv_ml453d
#win32:CONFIG(debug, debug|release): LIBS+=-L"$$OPENCV_451_PATH\\build_mingw\\lib\\" -lopencv_flann453d
#win32:CONFIG(debug, debug|release): LIBS+=-L"$$OPENCV_451_PATH\\build_mingw\\lib\\" -lopencv_imgproc453d
#win32:CONFIG(debug, debug|release): LIBS+=-L"$$OPENCV_451_PATH\\build_mingw\\lib\\" -lopencv_photo453d
#win32:CONFIG(debug, debug|release): LIBS+=-L"$$OPENCV_451_PATH\\build_mingw\\lib\\" -lopencv_features2d453d
#win32:CONFIG(debug, debug|release): LIBS+=-L"$$OPENCV_451_PATH\\build_mingw\\lib\\" -lopencv_imgcodecs453d
#win32:CONFIG(debug, debug|release): LIBS+=-L"$$OPENCV_451_PATH\\build_mingw\\lib\\" -lopencv_video453d
##win32:CONFIG(debug, debug|release): LIBS+=-L"$$OPENCV_451_PATH\\build_mingw\\lib\\" -lopencv_highgui453d
#win32:CONFIG(debug, debug|release): LIBS+=-L"$$OPENCV_451_PATH\\build_mingw\\lib\\" -lopencv_ts453d
#win32:CONFIG(debug, debug|release): LIBS+=-L"$$OPENCV_451_PATH\\build_mingw\\lib\\" -lopencv_calib3d453d
#win32:CONFIG(debug, debug|release): LIBS+=-L"$$OPENCV_451_PATH\\build_mingw\\lib\\" -lopencv_objdetect453d
#win32:CONFIG(debug, debug|release): LIBS+=-L"$$OPENCV_451_PATH\\build_mingw\\lib\\" -lopencv_stitching453d
#win32:CONFIG(debug, debug|release): LIBS+=-L"$$OPENCV_451_PATH\\build_mingw\\lib\\" -lopencv_dnn453d
#win32:CONFIG(debug, debug|release): LIBS+=-L"$$OPENCV_451_PATH\\build_mingw\\lib\\" -lopencv_video453d
#win32:CONFIG(debug, debug|release): LIBS+="$$(PYTHON_PATH)\\libs\\python39_d



#INCLUDEPATH += "C:/Program Files (x86)/Windows Kits/10/Include/10.0.22000.0/ucrt"
#LIBS += -L"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22000.0/ucrt/x64"

SOURCES += main.cpp \
           dialog.cpp  \
           keybertwrapper.cpp \
           mainwindow.cpp \
           print.cpp \
           shared.cpp \
           vccpparser.cpp \
           videoscreen.cpp

HEADERS += dialog.h \
    keybertwrapper.h \
    mainwindow.h \
    print.h \
    shared.h \
    timer.h \
    vccpparser.h \
    videoscreen.h

# Forms and resources
FORMS += dialog.ui \
    mainwindow.ui


EXAMPLE_FILES = *.png
QMAKE_CXXFLAGS += -v -v
# install
target.path = $$[QT_INSTALL_EXAMPLES]/corelib/ipc/sharedmemory
INSTALLS += target

# APP7 LIBRARY




win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/app7-master/app7CoreLib/release/ -lapp7CoreLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/app7-master/app7CoreLib/debug/ -lapp7CoreLib

INCLUDEPATH += $$PWD/libs/app7-master/include
DEPENDPATH += $$PWD/libs/app7-master/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libs/app7-master/app7CoreLib/release/libapp7CoreLib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libs/app7-master/app7CoreLib/debug/libapp7CoreLib.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libs/app7-master/app7CoreLib/release/app7CoreLib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libs/app7-master/app7CoreLib/debug/app7CoreLib.lib

win32: LIBS += -Ld:/Qt/Qt5.12.10/Tools/mingw730_64/x86_64-w64-mingw32/lib/ -lopengl32
win32: LIBS += -lglu32
INCLUDEPATH += -Ld:/Qt/Qt5.12.10/Tools/mingw730_64/x86_64-w64-mingw32/include
DEPENDPATH += -Ld:/Qt/Qt5.12.10/Tools/mingw730_64/x86_64-w64-mingw32/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/3PartyLibs/freeglut/lib/ -llibfreeglut.dll
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/3PartyLibs/freeglut/lib/ -llibfreeglut.dlld

INCLUDEPATH += $$PWD/libs/3PartyLibs/freeglut/include
DEPENDPATH += $$PWD/libs/3PartyLibs/freeglut/include
