#-------------------------------------------------
#
# Project created by QtCreator 2015-05-30T22:07:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ClientUser
TEMPLATE = app


win32:RC_ICONS += logo_ujaen.ico

LIBS += -static-libgcc -static-libstdc++

LIBS += "C:\Users\Linkku\Documents\ClientUser\libssh\libssh.dll"

#DEPENDPATH += libssh
#INCLUDEPATH += libssh

#CONFIG += D_BSD_SOURCE
#CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    dateClass.h \
    dirClass.h \
    SSHFunctions.h

FORMS    += mainwindow.ui


