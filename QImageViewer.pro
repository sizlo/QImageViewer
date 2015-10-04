#-------------------------------------------------
#
# Project created by QtCreator 2015-08-06T19:11:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QImageViewer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        session.cpp \
        sessionwindow.cpp \
    newsessiondialog.cpp

HEADERS  += mainwindow.h \
        session.h \
        sessionwindow.h \
    newsessiondialog.h

FORMS    += mainwindow.ui \
        sessionwindow.ui \
    newsessiondialog.ui
