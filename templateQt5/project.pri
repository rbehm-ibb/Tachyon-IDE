MOC_DIR = .build/moc
UI_DIR = .build/ui
OBJECTS_DIR = .build/obj
RCC_DIR = .build/qrc
DESTDIR=./bin
CONFIG += warn_on qt precompile_header
QMAKE_CXXFLAGS += -std=gnu++11

PRECOMPILED_HEADER = $$PWD/project.h
INCLUDEPATH += . $$PWD
HEADERS += $$PWD/project.h
