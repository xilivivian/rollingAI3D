#-------------------------------------------------
#
# Project created by QtCreator 2022-01-04T14:46:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets serialport

TARGET = check3d_32_64
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

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        mimageview.cpp \
        model/mmodel.cpp \
        task/ConsoleExample/FrameBuffer.cpp \
        task/algorithmCC/main_recognise_for_point_cloud.cpp \
        task/algorithmCC/main_recognise_for_point_cloud_2D.cpp \
        task/algorithmCC/main_recognise_for_point_cloud_3D.cpp \
        task/algorithmCC/point_cloud_recognise_function.cpp \
        task/camera3d.cpp \
        task/emitobject.cpp \
        task/taskallocation.cpp \
        task/threedtasks.cpp

HEADERS += \
        mainwindow.h \
        mimageview.h \
        model/mmodel.h \
        task/API/Callback.h \
        task/API/CameraDll.h \
        task/API/CameraStatus.h \
        task/API/PeakStructure.h \
        task/API/VevoParameterDefinitions.h \
        task/ConsoleExample/ConsoleExample.h \
        task/ConsoleExample/FrameBuffer.h \
        task/algorithmCC/main_recognise_for_point_cloud.h \
        task/algorithmCC/main_recognise_for_point_cloud_2D.h \
        task/algorithmCC/main_recognise_for_point_cloud_3D.h \
        task/camera3d.h \
        task/emitobject.h \
        task/resultdata.h \
        task/taskallocation.h \
        task/threedtasks.h

FORMS += \
        mainwindow.ui


INCLUDEPATH +=E:\MSVC32_build\install\include
E:\MSVC32_build\install\include\opencv2


#LIBS += "E:\Focalspec\FocalSpec Software Development Kit\API\Vevo.lib"
LIBS += "E:\Focalspec\FocalSpec Software Development Kit\API\Vevo.lib"

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -LE:/MSVC32_build/install/x86/vc15/lib/ -lopencv_world411
else:win32:CONFIG(debug, debug|release): LIBS += -LE:/MSVC32_build/install/x86/vc15/lib/ -lopencv_world411d
else:unix: LIBS += -LE:/MSVC32_build/install/x86/vc15/lib/ -lopencv_world411

INCLUDEPATH += E:/MSVC32_build/install/x86/vc15
DEPENDPATH += E:/MSVC32_build/install/x86/vc15


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
