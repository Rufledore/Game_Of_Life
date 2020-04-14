#-------------------------------------------------
#
# Project created by QtCreator 2018-12-13T15:05:06
#
#-------------------------------------------------

QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Simple Epidemic Simulator
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

TARGET = Simple-Epidemic-Simulator
TEMPLATE = app

win32:RC_ICONS += Resources/simulation-icon.ico
win32:VERSION = 1.0.0
win32:QMAKE_TARGET_DESCRIPTION =Epidemic Simulator
win32:QMAKE_TARGET_COPYRIGHT = All rights reserved.

SOURCES += \
    globals.cpp \
        main.cpp \
    mainwindow.cpp \
    person.cpp \
    populationmap.cpp \
    randomgenerator.cpp \
    simulationcore.cpp \
    simulationrunner.cpp

HEADERS += \
    globals.h \
    mainwindow.h \
    person.h \
    populationmap.h \
    randomgenerator.h \
    simulationcore.h \
    simulationrunner.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
