# // ******************************************************
# // * copyright (C) 2015 by Reinhardt Behm/rbehm@hushmail.com
# // * All Rights reserved
# // * File: templ5.pro
# // * created 2014/07/01 by rbehm
# // ******************************************************

TEMPLATE = app
include( $$[PRTEMPLATE]/project.pri )
include( $$[PRTEMPLATE]/lib/utils/stdicons.pri )
include( $$[PRTEMPLATE]/lib/utils/ibb-logo.pri )
include( $$[PRTEMPLATE]/lib/utils/config.pri )
include( $$[PRTEMPLATE]/lib/utils/toolbarspacer.pri )
include( $$[PRTEMPLATE]/lib/utils/ledicon/ledicon.pri )
include( $$[PRTEMPLATE]/lib/serialport/ibserialport.pri )

# select which ones are needed
QT += core gui widgets

HEADERS += \
    main.h \
    mainwindow.h \
    prop.h \
    console.h \
    finddialog.h \
    wordsmodel.h \
    wordsview.h

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    prop.cpp \
    console.cpp \
    finddialog.cpp \
    wordsmodel.cpp \
    wordsview.cpp

RESOURCES += \
    icons.qrc

OTHER_FILES += \
    styles.css

FORMS += \

