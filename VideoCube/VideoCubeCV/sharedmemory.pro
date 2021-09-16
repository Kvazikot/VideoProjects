QT += widgets

SOURCES += main.cpp \
           dialog.cpp  \
           shared.cpp

HEADERS += dialog.h \
    shared.h

# Forms and resources
FORMS += dialog.ui

EXAMPLE_FILES = *.png

# install
target.path = $$[QT_INSTALL_EXAMPLES]/corelib/ipc/sharedmemory
INSTALLS += target
