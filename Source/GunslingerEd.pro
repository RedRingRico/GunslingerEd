QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET_NAME = GunslingerEd

QMAKE_CXXFLAGS += -std=c++11

Release {
TARGET = ../Bin/$${TARGET_NAME}
OBJECTS_DIR = ../Obj/Release
MOC_DIR = ../Obj/Release
QMAKE_CXXFLAGS += -DBUILD_RELEASE
}

Debug {
TARGET = ../Bin/$${TARGET_NAME}D
OBJECTS_DIR = ../Obj/Debug
MOC_DIR = ../Obj/Debug
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS += -g -ggdb -DBUILD_DEBUG
}

TEMPLATE = app


SOURCES +=\
	Source/MainWindow.cpp \
    Source/Main.cpp

INCLUDEPATH += ./Headers

HEADERS  +=\
	Headers/MainWindow.h

versioninfo.target = Headers/GitVersion.h
versioninfo.commands = @mkdir -p Headers;\
						./GitVersion.sh ./Headers/GitVersion.h $(TARGET_NAME)
versioninfo.depends = ../.git

QMAKE_EXTRA_TARGETS = versioninfo

PRE_TARGETDEPS += Headers/GitVersion.h
