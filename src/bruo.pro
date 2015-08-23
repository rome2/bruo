#-------------------------------------------------
#
# Project created by QtCreator 2009-07-13T23:24:15
#
#-------------------------------------------------

QT += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = bruo
TEMPLATE = app

SOURCES += \
    rtaudio/RtAudio.cpp \
    audio/audiosnippet.cpp \
    audio/audiosystem.cpp \
    audio/audiotools.cpp \
    audio/peakdata.cpp \
    audio/peakthread.cpp \
    audio/samplebuffer.cpp \
    audio/sndfilesnippet.cpp \
    bruo.cpp \
    commands/appundocommand.cpp \
    commands/clearselectioncommand.cpp \
    commands/selectcommand.cpp \
    commands/selectingcommand.cpp \
    controls/bookmarkwidget.cpp \
    controls/shortcutedit.cpp \
    controls/stringselectdialog.cpp \
    controls/waveeditview.cpp \
    controls/wavemdiwindow.cpp \
    controls/waveoverview.cpp \
    controls/waveruler.cpp \
    controls/wavescales.cpp \
    controls/waveview.cpp \
    document.cpp \
    documentmanager.cpp \
    main.cpp\
    mainframe.cpp \
    rack/rack.cpp \
    rack/rackdevice.cpp \
    rack/rackinput.cpp \
    rack/rackoutput.cpp \
    rack/rackwindow.cpp \
    settings/isettingspage.cpp \
    settings/keymap.cpp \
    settings/loggingsystem.cpp \
    settings/settingsdialog.cpp \
    settings/shortcutdialog.cpp \
    toolwindows/browsertoolwindow.cpp \
    toolwindows/debugtoolwindow.cpp \
    toolwindows/historytoolwindow.cpp \
    rack/rackdevicegui.cpp \
    rack/rackoutputgui.cpp \
    controls/imagewidget.cpp \
    controls/imagedial.cpp \
    controls/imagebutton.cpp \
    controls/imageled.cpp \
    controls/imagetoggle.cpp \
    dsp/vumeter.cpp \
    controls/imagevu.cpp \
    actions/aboutaction.cpp \
    actions/exitaction.cpp \
    actions/gohomeaction.cpp \
    actions/aboutqtaction.cpp \
    actions/undoaction.cpp \
    actions/redoaction.cpp \
    actions/clearundoaction.cpp \
    actions/selectionaction.cpp \
    actions/cutaction.cpp \
    actions/copyaction.cpp \
    actions/deleteaction.cpp \
    actions/selectnothingaction.cpp \
    actions/extendselectiontostartaction.cpp \
    actions/extendselectiontoendaction.cpp \
    actions/extendselectiontocursoraction.cpp

HEADERS += \
    rtaudio/RtAudio.h \
    audio/audiosnippet.h \
    audio/audiosystem.h \
    audio/audiotools.h \
    audio/peakdata.h \
    audio/peakthread.h \
    audio/samplebuffer.h \
    audio/sndfilesnippet.h \
    bruo.h \
    commands/appundocommand.h \
    commands/clearselectioncommand.h \
    commands/selectcommand.h \
    commands/selectingcommand.h \
    controls/bookmarkwidget.h \
    controls/shortcutedit.h \
    controls/stringselectdialog.h \
    controls/waveeditview.h \
    controls/wavemdiwindow.h \
    controls/waveoverview.h \
    controls/waveruler.h \
    controls/wavescales.h \
    controls/waveview.h \
    document.h \
    documentmanager.h \
    mainframe.h \
    rack/rack.h \
    rack/rackdevice.h \
    rack/rackinput.h \
    rack/rackoutput.h \
    rack/rackwindow.h \
    settings/isettingspage.h \
    settings/keymap.h \
    settings/loggingsystem.h \
    settings/settingsdialog.h \
    settings/shortcutdialog.h \
    toolwindows/browsertoolwindow.h \
    toolwindows/debugtoolwindow.h \
    toolwindows/historytoolwindow.h \
    version.h \
    rack/rackdevicegui.h \
    rack/rackoutputgui.h \
    controls/imagebutton.h \
    controls/imagedial.h \
    controls/imageled.h \
    controls/imagetoggle.h \
    controls/imagewidget.h \
    dsp/vumeter.h \
    controls/imagevu.h \
    actions/aboutaction.h \
    actions/actions.h \
    actions/exitaction.h \
    actions/gohomeaction.h \
    actions/aboutqtaction.h \
    actions/undoaction.h \
    actions/redoaction.h \
    actions/clearundoaction.h \
    actions/selectionaction.h \
    actions/cutaction.h \
    actions/copyaction.h \
    actions/deleteaction.h \
    actions/selectnothingaction.h \
    actions/extendselectiontostartaction.h \
    actions/extendselectiontoendaction.h \
    actions/extendselectiontocursoraction.h

RESOURCES = bruo.qrc

FORMS +=

INCLUDEPATH += "$$PWD/rtaudio/include"

win* {
    DEFINES += __WINDOWS_MM__
    LIBS += -lwinmm
    LIBS += -lole32
    LIBS += -loleaut32
    LIBS += -ldsound
    LIBS += "$$PWD/../libsndfile/lib/libsndfile-1.lib"
    RC_FILE = bruo.rc
    INCLUDEPATH += "$$PWD/../libsndfile/include"
    DEFINES += __WINDOWS_ASIO__
    DEFINES += __WINDOWS_WASAPI__
    DEFINES += __WINDOWS_DS__
}

linux* {
    DEFINES += __UNIX_JACK__
    DEFINES += __LINUX_PULSE__
    DEFINES += __LINUX_ALSA__
    DEFINES += __LINUX_ALSASEQ__
    DEFINES += AVOID_TIMESTAMPING
    DEFINES += ALSA_PCM_NEW_HW_PARAMS_API
    CONFIG += link_pkgconfig \
        x11
    PKGCONFIG += alsa
    PKGCONFIG += sndfile
    PKGCONFIG += flac
    PKGCONFIG += vorbis
    PKGCONFIG += jack
    PKGCONFIG += libpulse-simple
}

CONFIG(debug, debug|release) {
    DEFINES += __RTMIDI_DEBUG__
    DEFINES += __RTAUDIO_DEBUG__
}
