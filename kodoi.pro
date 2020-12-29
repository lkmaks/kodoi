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
        common/AbstractBoard.cpp \
        common/BoardLayout.cpp \
        common/BoardPainter.cpp \
        common/BoardScene.cpp \
        common/BoardView.cpp \
        common/ColorBar.cpp \
        common/EngineViewer.cpp \
        common/EngineWrapper.cpp \
        common/Enums.cpp \
        common/ExplorerTree.cpp \
        common/InfoWidget.cpp \
        modules/drill/DrillInfoWidget.cpp \
        modules/drill/DrillMainWidget.cpp \
        modules/drill/DrillMode.cpp \
        modules/drill/BoardDrillContextStorage.cpp \
        modules/explorer/BoardContextStorage.cpp \
        modules/explorer/ExplorerMode.cpp \
        modules/explorer/MainWidget.cpp \
        MainWindow.cpp \
        main.cpp

HEADERS += \
        common/AbstractBoard.h \
        common/BoardLayout.h \
        common/BoardPainter.h \
        common/BoardScene.h \
        common/BoardView.h \
        common/ColorBar.h \
        common/EngineViewer.h \
        common/EngineWrapper.h \
        common/Enums.h \
        common/ExplorerTree.h \
        common/InfoWidget.h \
        modules/Settings.h \
        modules/drill/BoardDrillContextStorage.h \
        modules/drill/DrillInfoWidget.h \
        modules/drill/DrillMainWidget.h \
        modules/drill/DrillMode.h \
    modules/drill/DrillTools.h \
        modules/explorer/BoardContextStorage.h \
        modules/explorer/ExplorerMode.h \
    modules/explorer/ExplorerTools.h \
        modules/explorer/MainWidget.h \
        Config.h \
        MainWindow.h \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

