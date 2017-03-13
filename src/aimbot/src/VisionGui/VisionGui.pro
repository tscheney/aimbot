#-------------------------------------------------
#
# Project created by QtCreator 2017-03-11T19:34:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VisionGui
TEMPLATE = app

INCLUDEPATH += /usr/include/
LIBS += -I/usr/lib/x86_64-linux-gnu/ -lopencv_core -lopencv_imgcodecs -lopencv_highgui
#LIBS += -L/usr/lib/x86_64-linux-gnu/ -lopencv_core -lopencv_highgui

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    cvimagewidget.h

FORMS    += mainwindow.ui
