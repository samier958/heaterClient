#-------------------------------------------------
#
# Project created by QtCreator 2015-01-21T15:44:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = heaterClient
TEMPLATE = app

LIBS += $${PWD}/libmodbus-5.dll

DEFINES += QMODBUS_LIBRARY

SOURCES += main.cpp \
    cheatermainform.cpp \
    cheaterrealtimedata.cpp \
    cheaterclient.cpp \
    cheaterparametersettings.cpp \
    cheaterfaultinfo.cpp \
    cheaterhistoryrecord.cpp \
    qmodbusbits.cpp \
    qmodbuserror.cpp \
    qmodbusmaster.cpp \
    qmodbusregisters.cpp

HEADERS  += \
    cheatermainform.h \
    cheaterrealtimedata.h \
    cheaterclient.h \
    cheaterparametersettings.h \
    cheaterfaultinfo.h \
    cheaterhistoryrecord.h \
    qmodbus.h \
    qmodbusbits.h \
    qmodbuserror.h \
    qmodbusmaster.h \
    qmodbusregisters.h \
    modbus.h \
    config.h

FORMS += \
    cheatermainform.ui \
    cheaterrealtimedata.ui \
    cheaterclient.ui \
    cheaterparametersettings.ui \
    cheaterfaultinfo.ui \
    cheaterhistoryrecord.ui

CONFIG += warn_off

RC_FILE = logo.rc
