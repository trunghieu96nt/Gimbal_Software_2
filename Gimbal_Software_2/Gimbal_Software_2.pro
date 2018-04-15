#-------------------------------------------------
#
# Project created by QtCreator 2017-10-03T15:03:52
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Gimbal_Software_2
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    serialport.cpp \
    imageprocessingthread.cpp \
    cqtopencvviewergl.cpp \
    pid.cpp \
    imageprocessor.cpp

HEADERS += \
        mainwindow.h \
    serialport.h \
    imageprocessingthread.h \
    cqtopencvviewergl.h \
    pid.h \
    imageprocessor.h

win32 {
    INCLUDEPATH += C:\opencv331\opencv-build\install\include
    LIBS += C:\opencv331\opencv-build\bin\libopencv_core331.dll
    LIBS += C:\opencv331\opencv-build\bin\libopencv_highgui331.dll
    LIBS += C:\opencv331\opencv-build\bin\libopencv_imgcodecs331.dll
    LIBS += C:\opencv331\opencv-build\bin\libopencv_imgproc331.dll
    LIBS += C:\opencv331\opencv-build\bin\libopencv_features2d331.dll
    LIBS += C:\opencv331\opencv-build\bin\libopencv_calib3d331.dll
    LIBS += C:\opencv331\opencv-build\bin\libopencv_tracking331.dll
    LIBS += C:\opencv331\opencv-build\bin\libopencv_videoio331.dll

    #INCLUDEPATH += D:\opencv3_3\opencv\build_qt\install\include
    #LIBS += D:\opencv3_3\opencv\build_qt\bin\libopencv_core330.dll
    #LIBS += D:\opencv3_3\opencv\build_qt\bin\libopencv_highgui330.dll
    #LIBS += D:\opencv3_3\opencv\build_qt\bin\libopencv_imgcodecs330.dll
    #LIBS += D:\opencv3_3\opencv\build_qt\bin\libopencv_imgproc330.dll
    #LIBS += D:\opencv3_3\opencv\build_qt\bin\libopencv_features2d330.dll
    #LIBS += D:\opencv3_3\opencv\build_qt\bin\libopencv_calib3d330.dll
    #LIBS += D:\opencv3_3\opencv\build_qt\bin\libopencv_tracking330.dll
    #LIBS += D:\opencv3_3\opencv\build_qt\bin\libopencv_videoio330.dll
}

linux-g++ | linux-g++-64 | linux-g++-32 {
    INCLUDEPATH += ~/opencv3_3/opencv/build_ubuntu/include

    LIBS += ~/opencv3_3/opencv/build_ubuntu/lib/libopencv_core.so
    LIBS += ~/opencv3_3/opencv/build_ubuntu/lib/libopencv_highgui.so
    LIBS += ~/opencv3_3/opencv/build_ubuntu/lib/libopencv_imgcodecs.so
    LIBS += ~/opencv3_3/opencv/build_ubuntu/lib/libopencv_imgproc.so
    LIBS += ~/opencv3_3/opencv/build_ubuntu/lib/libopencv_features2d.so
    LIBS += ~/opencv3_3/opencv/build_ubuntu/lib/libopencv_tracking.so
    LIBS += ~/opencv3_3/opencv/build_ubuntu/lib/libopencv_videoio.so
}

FORMS += \
        mainwindow.ui

RESOURCES += \
    resource.qrc

RC_ICONS = icon.ico
