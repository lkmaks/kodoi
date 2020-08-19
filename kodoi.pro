#-------------------------------------------------
#
# Project created by QtCreator 2019-07-17T14:21:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = kodoi
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

SOURCES += \
        AbstractBoard.cpp \
        BasicEngineViewer.cpp \
        BoardContextStorage.cpp \
        BoardPainter.cpp \
        BoardScene.cpp \
        BoardView.cpp \
        EngineWrapper.cpp \
        Enums.cpp \
        ExplorerMode.cpp \
        ExplorerTree.cpp \
        MainWidget.cpp \
        MainWindow.cpp \
        main.cpp

HEADERS += \
        AbstractBoard.h \
        BasicEngineViewer.h \
        BoardContextStorage.h \
        BoardPainter.h \
        BoardScene.h \
        BoardView.h \
        Config.h \
        EngineWrapper.h \
        Enums.h \
        ExplorerMode.h \
        ExplorerTree.h \
        MainWidget.h \
        MainWindow.h \
        Settings.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
