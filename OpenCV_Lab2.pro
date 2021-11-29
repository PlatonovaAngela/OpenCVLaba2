TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp

INCLUDEPATH += D:\opencv\build\include

LIBS += D:\opencv_build\bin\libopencv_core454.dll
LIBS += D:\opencv_build\bin\libopencv_highgui454.dll
LIBS += D:\opencv_build\bin\libopencv_imgcodecs454.dll
LIBS += D:\opencv_build\bin\libopencv_imgproc454.dll
LIBS += D:\opencv_build\bin\libopencv_features2d454.dll
LIBS += D:\opencv_build\bin\libopencv_calib3d454.dll
