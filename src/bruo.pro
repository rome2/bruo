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
    peakdata.cpp \
    waveview.cpp \
    loggingsystem.cpp \
    commands/selectingcommand.cpp \
    commands/selectcommand.cpp \
    commands/appundocommand.cpp \
    controls/shortcutedit.cpp \
    wavemdiwindow.cpp \
    settings/shortcutdialog.cpp \
    settings/keymap.cpp \
    controls/stringselectdialog.cpp \
    settings/isettingspage.cpp \
    settings/settingsdialog.cpp \
    controls/bookmarkwidget.cpp \
    toolwindows/historytoolwindow.cpp \
    toolwindows/debugtoolwindow.cpp \
    toolwindows/browsertoolwindow.cpp \
    audio/samplebuffer.cpp

HEADERS += mainframe.h \
    bruo.h \
    waveview.h \
    documentmanager.h \
    document.h \
    peakdata.h \
    loggingsystem.h \
    commands/selectingcommand.h \
    commands/selectcommand.h \
    commands/appundocommand.h \
    controls/shortcutedit.h \
    wavemdiwindow.h \
    settings/shortcutdialog.h \
    settings/keymap.h \
    controls/stringselectdialog.h \
    settings/isettingspage.h \
    settings/settingsdialog.h \
    controls/bookmarkwidget.h \
    toolwindows/historytoolwindow.h \
    toolwindows/debugtoolwindow.h \
    toolwindows/browsertoolwindow.h \
    version.h \
    audio/samplebuffer.h

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
