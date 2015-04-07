QT += core gui widgets qml quick

TEMPLATE = app
CONFIG += flat

INCLUDEPATH += qjack

LIBS += -L -lqjackaudio \
                -ljack \
                -lfftw3

SOURCES += \
    main.cpp \
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

RESOURCES += \
    resources.qrc

