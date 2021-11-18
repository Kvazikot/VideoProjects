cmake_minimum_required(VERSION 3.18)

project(VideoCubeCV)
set( CMAKE_VERBOSE_MAKEFILE on )

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


set( OpenCV_DIR "D:\\opencv\\build2" )
set( Qt5_DIR "D:\\Qt\\Qt5.12.10\\" )

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

find_package(OpenCV REQUIRED )
#find_package(TBB REQUIRED)
# Widgets finds its own dependencies (QtGui and QtCore).

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


# The Qt5Widgets_INCLUDES also includes the include directories for
# dependencies QtCore and QtGui
include_directories(${Qt5Widgets_INCLUDES})
include_directories(${Qt5Concurrent_INCLUDES})
# We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 5.
add_definitions(${Qt5Widgets_DEFINITIONS})
set(CMAKE_BUILD_TYPE Release)

set(SOURCES main.cpp 
           dialog.cpp  
           print.cpp
           shared.cpp
           videoscreen.cpp
)


set ( UI_HEADERS
   ui_dialog.h
)


include_directories(.)

set ( UIS
 dialog.ui
 mainwindow_ver1.ui
)

#set(HEADERS_MOC videoscreen.h)

set(HEADERS dialog.h 
    shared.h
    videoscreen.h
    print.h
    ui_dialog.h
)

QT5_WRAP_CPP(HEADERS_MOC ${HEADERS_MOC})
QT5_WRAP_UI(UI_HEADERS ${UIS})

# Forms and resources
#FORMS += dialog.ui

#include_directories(${DIR}/headers /usr/include)
#link_directories (${DIR}/src/utils /opt/local/lib)
add_executable(${PROJECT_NAME} ${UI_HEADERS} ${SOURCES} ${HEADERS} )
# The Qt5Widgets_LIBRARIES variable also includes QtGui and QtCore
target_link_libraries(${PROJECT_NAME} ${Qt5Widgets_LIBRARIES} ${Qt5Concurrent_LIBRARIES})
target_link_libraries(${PROJECT_NAME} TBB::tbb)

add_definitions(-DUSE_AVX_INSTRUCTIONS=1)

#TARGET_LINK_LIBRARIES(VideoCubeCV opencv_core opencv_imgproc opencv_video)
target_link_libraries( VideoCubeCV ${OpenCV_LIBS} )
