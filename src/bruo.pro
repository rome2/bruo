#-------------------------------------------------
#
# Project created by QtCreator 2009-07-13T23:24:15
#
#-------------------------------------------------

QT += xml widgets gui

TARGET = bruo
TEMPLATE = app

SOURCES += main.cpp\
    mainframe.cpp \
    bruo.cpp \
    documentmanager.cpp \
    document.cpp \
    historytoolwindow.cpp \
    peakdata.cpp \
    waveview.cpp \
    stringselectdialog.cpp \
    debugtoolwindow.cpp \
    loggingsystem.cpp \
    commands/selectingcommand.cpp \
    commands/selectcommand.cpp \
    commands/appundocommand.cpp \
    shortcutdialog.cpp \
    controls/shortcutedit.cpp \
    keymap.cpp \
    wavemdiwindow.cpp

HEADERS += mainframe.h \
    bruo.h \
    waveview.h \
    documentmanager.h \
    document.h \
    historytoolwindow.h \
    peakdata.h \
    stringselectdialog.h \
    debugtoolwindow.h \
    loggingsystem.h \
    commands/selectingcommand.h \
    commands/selectcommand.h \
    commands/appundocommand.h \
    shortcutdialog.h \
    controls/shortcutedit.h \
    keymap.h \
    wavemdiwindow.h

RESOURCES = bruo.qrc

FORMS +=

win* {
    DEFINES += __WINDOWS_MM__
    LIBS += -lwinmm
    RC_FILE = bruo.rc
}

linux* {
    DEFINES += __LINUX_ALSA__
    DEFINES += __LINUX_ALSASEQ__
    DEFINES += AVOID_TIMESTAMPING
    CONFIG += link_pkgconfig \
        x11
    PKGCONFIG += alsa
}

CONFIG(debug, debug|release) {
    DEFINES += __RTMIDI_DEBUG__
}
