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
    appundocommand.cpp \
    selectingcommand.cpp \
    selectcommand.cpp \
    historytoolwindow.cpp \
    peakdata.cpp \
    waveview.cpp \
    recentfilesdialog.cpp

HEADERS += mainframe.h \
    bruo.h \
    waveview.h \
    appundocommand.h \
    selectcommand.h \
    selectingcommand.h \
    documentmanager.h \
    document.h \
    historytoolwindow.h \
    peakdata.h \
    recentfilesdialog.h

RESOURCES = bruo.qrc

FORMS +=

win* {
    DEFINES += __WINDOWS_MM__
    LIBS += -lwinmm
}

linux* {
    DEFINES += __LINUX_ALSA__
    DEFINES += __LINUX_ALSASEQ__
    DEFINES += AVOID_TIMESTAMPING
    CONFIG += link_pkgconfig \
        x11
    PKGCONFIG += alsa
}

debug:DEFINES += __RTMIDI_DEBUG__
