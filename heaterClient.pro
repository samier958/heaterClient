#-------------------------------------------------
#
# Project created by QtCreator 2015-01-21T15:44:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = heaterClient
TEMPLATE = app


SOURCES += main.cpp \
    cheatermainform.cpp \
    cheaterrealtimedata.cpp \
    cheaterclient.cpp \
    cheaterparametersettings.cpp \
    cheaterfaultinfo.cpp \
    cheaterhistoryrecord.cpp

HEADERS  += \
    cheatermainform.h \
    cheaterrealtimedata.h \
    cheaterclient.h \
    cheaterparametersettings.h \
    cheaterfaultinfo.h \
    cheaterhistoryrecord.h

FORMS += \
    cheatermainform.ui \
    cheaterrealtimedata.ui \
    cheaterclient.ui \
    cheaterparametersettings.ui \
    cheaterfaultinfo.ui \
    cheaterhistoryrecord.ui
