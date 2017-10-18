# // ******************************************************
# // * copyright (C) 2015 by Reinhardt Behm/rbehm@hushmail.com
# // * All Rights reserved
# // * created 2017/07/01 by rbehm
# // ******************************************************

TEMPLATE = app
include( templateQt5/project.pri )
include( templateQt5/lib/utils/stdicons.pri )
include( templateQt5/lib/utils/ibb-logo.pri )
include( templateQt5/lib/utils/config.pri )
include( templateQt5/lib/utils/toolbarspacer.pri )
include( templateQt5/lib/utils/ledicon/ledicon.pri )
include( templateQt5/lib/serialport/ibserialport.pri )

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

