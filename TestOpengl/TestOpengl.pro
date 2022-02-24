#-------------------------------------------------
#
# Project created by QtCreator 2021-09-07T09:20:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = playtest
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


INCLUDEPATH += $$PWD/include
INCLUDEPATH += /home/cftc/toolchain/host/aarch64-buildroot-linux-gnu/sysroot/usr/include/rockchip/
INCLUDEPATH += /home/cftc/toolchain/host/aarch64-buildroot-linux-gnu/sysroot/usr/include/libdrm
LIBS += -L$$PWD/lib/  -ldl -lz -lbz2 -lrockchip_mpp -lvproc_rga -ldrm -lpthread

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    CMPlayerInterface.cpp \
    mutex.cpp \
    vdec.cpp \
    rtsp/Base64EncDec.c \
    rtsp/md5.c \
    rtsp/ourMD5.c \
    rtsp/rtcp.c \
    rtsp/rtp.c \
    rtsp/rtsp.c \
    rtsp/rtspApi.c \
    rtsp/rtspComm.c \
    debugout/debug.c \
    testform.cpp \
    rgaapi.cpp \
    libdrm/bo.c \
    libdrm/dev.c \
    libdrm/modeset.c \
    libdrm/planetest.cpp

HEADERS += \
        mainwindow.h \
    CMPlayerInterface.h \
    CommonDefine.h \
    dhplay.h \
    mutex.h \
    vdec.h \
    rtsp/Base64EncDec.h \
    rtsp/md5.h \
    rtsp/mutex.h \
    rtsp/ourMD5.h \
    rtsp/rtcp.h \
    rtsp/rtp.h \
    rtsp/rtsp.h \
    rtsp/rtspApi.h \
    rtsp/rtspComm.h \
    rtsp/types.h \
    debugout/debug.h \
    testform.h \
    rgaapi.h \
    libdrm/bo.h \
    libdrm/dev.h \
    libdrm/modeset.h \
    libdrm/planetest.h

FORMS += \
        mainwindow.ui \
    testform.ui

DISTFILES += \
    rtsp/Base64EncDec.o \
    rtsp/md5.o \
    rtsp/ourMD5.o \
    rtsp/rtcp.o \
    rtsp/rtp.o \
    rtsp/rtsp.o \
    rtsp/rtspApi.o \
    rtsp/rtspComm.o \
