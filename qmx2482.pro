QT += core gui widgets qml
OBJECTS_DIR = obj
MOC_DIR = moc
DESTDIR = bin
TARGET = qmx2482
TEMPLATE = app
CONFIG -= console
CONFIG += flat

INCLUDEPATH += qjack

LIBS += -L -lqjackaudio \
                -ljack \
                -lfftw3

SOURCES += \
    mainwindow.cpp \
    main.cpp \
    channelwidget.cpp \
    mainmixerwidget.cpp \
    aboutdialog.cpp \
    qjack/qamplifier.cpp \
    qjack/qcompressor.cpp \
    qjack/qequalizer.cpp \
    qjack/qequalizercontrol.cpp \
    qjack/qfftw.cpp \
    qjack/qjackclient.cpp \
    qjack/qjackport.cpp \
    qjack/qnoisegate.cpp \
    qjack/qsamplebuffer.cpp \
    qjack/qsignalgenerator.cpp \
    qjack/qunits.cpp \
    src/channel.cpp \
    src/mixer.cpp

HEADERS += \
    mainwindow.h \
    channelwidget.h \
    mainmixerwidget.h \
    aboutdialog.h \
    qjack/qamplifier.h \
    qjack/qaudioprocessor.h \
    qjack/qcompressor.h \
    qjack/qdigitalfilter.h \
    qjack/qequalizer.h \
    qjack/qequalizercontrol.h \
    qjack/qfftw.h \
    qjack/qjackclient.h \
    qjack/qjackport.h \
    qjack/qnoisegate.h \
    qjack/qsamplebuffer.h \
    qjack/qsignalgenerator.h \
    qjack/qunits.h \
    src/channel.h \
    src/mixer.h

FORMS += \
    mainwindow.ui \
    channelwidget.ui \
    mainmixerwidget.ui \
    aboutdialog.ui

RESOURCES += \
    resources.qrc

