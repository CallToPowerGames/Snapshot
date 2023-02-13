QT       += core gui widgets opengl multimedia

TARGET = Snapshot
TEMPLATE = app

SOURCES += hcpp/main.cpp\
        hcpp/mainwindow.cpp \
        hcpp/opencvviewer.cpp \
        hcpp/textwindow.cpp

HEADERS  += hcpp/mainwindow.h \
        hcpp/opencvviewer.h \
        hcpp/libimgfilter.hpp \
        hcpp/textwindow.h

FORMS    += ui/mainwindow.ui \
        ui/textwindow.ui

macx: LIBS += -L$$PWD/../../../../../../../usr/local/Cellar/opencv/2.4.9/lib/ -lopencv_core
macx: LIBS += -L$$PWD/../../../../../../../usr/local/Cellar/opencv/2.4.9/lib/ -lopencv_highgui
macx: LIBS += -L$$PWD/../../../../../../../usr/local/Cellar/opencv/2.4.9/lib/ -lopencv_imgproc
macx: INCLUDEPATH += $$PWD/../../../../../../../usr/local/Cellar/opencv/2.4.9/include
macx: DEPENDPATH += $$PWD/../../../../../../../usr/local/Cellar/opencv/2.4.9/include

win32: LIBS += -L$$PWD/../../../../../../opencv/mingw/install/x64/mingw/lib/ -llibopencv_core249
win32: LIBS += -L$$PWD/../../../../../../opencv/mingw/install/x64/mingw/lib/ -llibopencv_highgui249
win32: LIBS += -L$$PWD/../../../../../../opencv/mingw/install/x64/mingw/lib/ -llibopencv_imgproc249
win32: INCLUDEPATH += $$PWD/../../../../../../opencv/mingw/install/include
win32: DEPENDPATH += $$PWD/../../../../../../opencv/mingw/install/include

RESOURCES += \
    resources/resources.qrc

win32 {
    RC_ICONS = Snapshot.ico
}
