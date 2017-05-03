#-------------------------------------------------
#
# Project created by QtCreator 2017-03-22T22:21:14
#
#-------------------------------------------------

QT       += core gui
QT       += printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SDI_Basic_Painting
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        sdi_mainwindow.cpp \
    lefttoolswidget.cpp \
    sdi_painter.cpp \
    draw2dwidget.cpp \
    sdi_point.cpp \
    matrixlibs.cpp \
    sdi_point3d.cpp \
    sdi_geometricshape.cpp

HEADERS  += sdi_mainwindow.h \
    lefttoolswidget.h \
    sdi_painter.h \
    draw2dwidget.h \
    sdi_point.h \
    matrixlibs.h \
    sdi_point3d.h \
    sdi_geometricshape.h

RESOURCES += \
    resource.qrc
RC_FILE = appinfo.rc
