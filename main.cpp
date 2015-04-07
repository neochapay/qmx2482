///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//    This file is part of QJackAudio.                                       //
//    Copyright (C) 2014 Jacob Dawid, jacob@omg-it.works                     //
//                                                                           //
//    QJackAudio is free software: you can redistribute it and/or modify     //
//    it under the terms of the GNU General Public License as published by   //
//    the Free Software Foundation, either version 3 of the License, or      //
//    (at your option) any later version.                                    //
//                                                                           //
//    QJackAudio is distributed in the hope that it will be useful,          //
//    but WITHOUT ANY WARRANTY; without even the implied warranty of         //
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          //
//    GNU General Public License for more details.                           //
//                                                                           //
//    You should have received a copy of the GNU General Public License      //
//    along with QJackAudio. If not, see <http://www.gnu.org/licenses/>.     //
//                                                                           //
//    It is possible to obtain a closed-source license of QJackAudio.        //
//    If you're interested, contact me at: jacob@omg-it.works                //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlContext>
#include <QAudioProcessor>

#include "qjack/qjackclient.h"
#include "qjack/qaudioprocessor.h"

#include "src/channel.h"
#include "src/mixer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QJackClient* jackClient = QJackClient::instance();
    QAudioProcessor *aprocessor = new QAudioProcessor();

    if(jackClient->connectToServer("QMX2482")) {
        jackClient->setAudioProcessor(aprocessor);
    }

    Mixer *mainMixer = new Mixer();
    Channel *chanel_1 = new Channel(1);
    Channel *chanel_2 = new Channel(2);
    Channel *chanel_3 = new Channel(3);
    Channel *chanel_4 = new Channel(4);
    Channel *chanel_5 = new Channel(5);
    Channel *chanel_6 = new Channel(6);
    Channel *chanel_7 = new Channel(7);
    Channel *chanel_8 = new Channel(8);

    mainMixer->registerChannel(1,chanel_1);
    mainMixer->registerChannel(2,chanel_2);
    mainMixer->registerChannel(3,chanel_3);
    mainMixer->registerChannel(4,chanel_4);
    mainMixer->registerChannel(5,chanel_5);
    mainMixer->registerChannel(6,chanel_6);
    mainMixer->registerChannel(7,chanel_7);
    mainMixer->registerChannel(8,chanel_8);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    engine.rootContext()->setContextProperty("mainMixer", mainMixer);
    engine.rootContext()->setContextProperty("channel_1", chanel_1);
    engine.rootContext()->setContextProperty("channel_2", chanel_2);
    engine.rootContext()->setContextProperty("channel_3", chanel_3);
    engine.rootContext()->setContextProperty("channel_4", chanel_4);
    engine.rootContext()->setContextProperty("channel_5", chanel_5);
    engine.rootContext()->setContextProperty("channel_6", chanel_6);
    engine.rootContext()->setContextProperty("channel_7", chanel_7);
    engine.rootContext()->setContextProperty("channel_8", chanel_8);

    return a.exec();
}
