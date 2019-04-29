#-------------------------------------------------
#
# Project created by QtCreator 2018-01-23T20:09:30
#
#-------------------------------------------------

QT       += core gui
RC_ICONS += Application.ico

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TRANSLATIONS += trans/en.ts

TARGET = WeLab
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

#CONFIG(debug, debug|release) {
#    DEFINES += PRIVILEGED
#}

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    errorMessage.cpp \
    jupodAdpter.cpp \
    VirtualComponentData.cpp \
    about.cpp \
    dialogConnnect.cpp \
    VirtualPanelData.cpp \
    pannelsetting.cpp

HEADERS += \
        mainwindow.h \
    errorMessage.h \
    jupodAdpter.h \
    VirtualComponentData.h \
    about.h \
    dialogConnnect.h \
    dll/JUDEBUG.h \
    VirtualPanelData.h \
    pannelsetting.h \
    signature.h \
    dll/JUTILITY.h

RESOURCES += \
    res.qrc

FORMS += \
    dialogConnnect.ui

if(contains(DEFINES,TRIAL_EDITION)){
    message("compile for trial edition")
    win32: LIBS += -L$$PWD/dll_trial/ -lJUDEBUG
    win32: LIBS += -L$$PWD/dll_trial/ -lJUTILITY
    INCLUDEPATH += $$PWD/dll_trial
    DEPENDPATH += $$PWD/dll_trial
}else{
    win32: LIBS += -L$$PWD/dll/ -lJUDEBUG
    win32: LIBS += -L$$PWD/dll/ -lJUTILITY
    INCLUDEPATH += $$PWD/dll
    DEPENDPATH += $$PWD/dll
}

RC_FILE += \
    version.rc
