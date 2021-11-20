QT += widgets core concurrent

OPENCV_451_PATH = "d:\\opencv"
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


win32:CONFIG(release, debug|release): LIBS+="$$OPENCV_451_PATH\\build2\\lib\\Release\\opencv_core453.lib"
win32:CONFIG(release, debug|release): LIBS+="$$OPENCV_451_PATH\\build2\\lib\\Release\\opencv_ml453.lib"
win32:CONFIG(release, debug|release): LIBS+="$$OPENCV_451_PATH\\build2\\lib\\Release\\opencv_flann453.lib"
win32:CONFIG(release, debug|release): LIBS+="$$OPENCV_451_PATH\\build2\\lib\\Release\\opencv_imgproc453.lib"
win32:CONFIG(release, debug|release): LIBS+="$$OPENCV_451_PATH\\build2\\lib\\Release\\opencv_photo453.lib"
win32:CONFIG(release, debug|release): LIBS+="$$OPENCV_451_PATH\\build2\\lib\\Release\\opencv_features2d453.lib"
win32:CONFIG(release, debug|release): LIBS+="$$OPENCV_451_PATH\\build2\\lib\\Release\\opencv_imgcodecs453.lib"
LIBS+="$$OPENCV_451_PATH\\build2\\lib\\Release\\opencv_videoio453.lib"
LIBS+="$$OPENCV_451_PATH\\build2\\lib\\Release\\opencv_highgui453.lib"
win32:CONFIG(release, debug|release): LIBS+="$$OPENCV_451_PATH\\build2\\lib\\Release\\opencv_ts453.lib"
win32:CONFIG(release, debug|release): LIBS+="$$OPENCV_451_PATH\\build2\\lib\\Release\\opencv_calib3d453.lib"
win32:CONFIG(release, debug|release): LIBS+="$$OPENCV_451_PATH\\build2\\lib\\Release\\opencv_objdetect453.lib"
win32:CONFIG(release, debug|release): LIBS+="$$OPENCV_451_PATH\\build2\\lib\\Release\\opencv_stitching453.lib"
win32:CONFIG(release, debug|release): LIBS+="$$OPENCV_451_PATH\\build2\\lib\\Release\\opencv_dnn453.lib"
win32:CONFIG(release, debug|release): LIBS+="$$OPENCV_451_PATH\\build2\\lib\\Release\\opencv_video453.lib"


win32:CONFIG(debug, debug|release): LIBS+="$$OPENCV_451_PATH\\build2\\lib\\Debug\\opencv_core453d.lib"
win32:CONFIG(debug, debug|release): LIBS+="$$OPENCV_451_PATH\\build2\\lib\\Debug\\opencv_ml453d.lib"
win32:CONFIG(debug, debug|release): LIBS+="$$OPENCV_451_PATH\\build2\\lib\\Debug\\opencv_flann453d.lib"
win32:CONFIG(debug, debug|release): LIBS+="$$OPENCV_451_PATH\\build2\\lib\\Debug\\opencv_imgproc453d.lib"
win32:CONFIG(debug, debug|release): LIBS+="$$OPENCV_451_PATH\\build2\\lib\\Debug\\opencv_photo453d.lib"
win32:CONFIG(debug, debug|release): LIBS+="$$OPENCV_451_PATH\\build2\\lib\\Debug\\opencv_features2d453d.lib"
win32:CONFIG(debug, debug|release): LIBS+="$$OPENCV_451_PATH\\build2\\lib\\Debug\\opencv_imgcodecs453d.lib"
win32:CONFIG(debug, debug|release): LIBS+="$$OPENCV_451_PATH\\build2\\lib\\Debug\\opencv_video453d.lib"
#win32:CONFIG(debug, debug|release): LIBS+="$$OPENCV_451_PATH\\build2\\lib\\Debug\\opencv_highgui453d.lib"
win32:CONFIG(debug, debug|release): LIBS+="$$OPENCV_451_PATH\\build2\\lib\\Debug\\opencv_ts453d.lib"
win32:CONFIG(debug, debug|release): LIBS+="$$OPENCV_451_PATH\\build2\\lib\\Debug\\opencv_calib3d453d.lib"
win32:CONFIG(debug, debug|release): LIBS+="$$OPENCV_451_PATH\\build2\\lib\\Debug\\opencv_objdetect453d.lib"
win32:CONFIG(debug, debug|release): LIBS+="$$OPENCV_451_PATH\\build2\\lib\\Debug\\opencv_stitching453d.lib"
win32:CONFIG(debug, debug|release): LIBS+="$$OPENCV_451_PATH\\build2\\lib\\Debug\\opencv_dnn453d.lib"
win32:CONFIG(debug, debug|release): LIBS+="$$OPENCV_451_PATH\\build2\\lib\\Debug\\opencv_video453d.lib"
QMAKE_CXXFLAGS_DEBUG+=-Od

INCLUDEPATH += "C:/Program Files (x86)/Windows Kits/10/Include/10.0.17763.0/ucrt"
LIBS += -L"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.17763.0/ucrt/x64"

SOURCES += main.cpp \
           dialog.cpp  \
           print.cpp \
           shared.cpp \
           videoscreen.cpp

HEADERS += dialog.h \
    print.h \
    shared.h \
    timer.h \
    videoscreen.h

# Forms and resources
FORMS += dialog.ui


EXAMPLE_FILES = *.png

# install
target.path = $$[QT_INSTALL_EXAMPLES]/corelib/ipc/sharedmemory
INSTALLS += target
