QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

SOURCES += \
    accounts.cpp \
    launchgame.cpp \
    main.cpp \
    mainwindow.cpp \
    params.cpp \
    readdata.cpp \
    uploadwindow.cpp

HEADERS += \
    accounts.h \
    launchgame.h \
    mainwindow.h \
    params.h \
    readdata.h \
    uploadwindow.h

FORMS += \
    mainwindow.ui \
    uploadwindow.ui

RESOURCES += \
    resources.qrc

QMAKE_LFLAGS += /MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\"

VERSION = 0.7.0.0
QMAKE_TARGET_DESCRIPTION = https://vk.com/siboost
QMAKE_TARGET_COMPANY = SiBoost Inc.
QMAKE_TARGET_PRODUCT = SiBoost
QMAKE_TARGET_COPYRIGHT = Copyright © 2021 SiBoost Inc.
RC_ICONS += "images/app.ico"
