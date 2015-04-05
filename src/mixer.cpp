#include "mixer.h"
#include "channel.h"
#include <QJackClient>

Mixer::Mixer(QObject *parent) : QObject(parent)
{
    QJackClient *jackClient = QJackClient::instance();
    _subGroup1Out = jackClient->registerAudioOutPort("subgroup1_out");
    _subGroup2Out = jackClient->registerAudioOutPort("subgroup2_out");
    _subGroup3Out = jackClient->registerAudioOutPort("subgroup3_out");
    _subGroup4Out = jackClient->registerAudioOutPort("subgroup4_out");
    _subGroup5Out = jackClient->registerAudioOutPort("subgroup5_out");
    _subGroup6Out = jackClient->registerAudioOutPort("subgroup6_out");
    _subGroup7Out = jackClient->registerAudioOutPort("subgroup7_out");
    _subGroup8Out = jackClient->registerAudioOutPort("subgroup8_out");

    _mainLeftOut = jackClient->registerAudioOutPort("main_out_1");
    _mainRightOut = jackClient->registerAudioOutPort("main_out_2");

    _subgroup1FaderStage = new QAmplifier();
    _subgroup2FaderStage = new QAmplifier();
    _subgroup3FaderStage = new QAmplifier();
    _subgroup4FaderStage = new QAmplifier();
    _subgroup5FaderStage = new QAmplifier();
    _subgroup6FaderStage = new QAmplifier();
    _subgroup7FaderStage = new QAmplifier();
    _subgroup8FaderStage = new QAmplifier();
    _main1FaderStage = new QAmplifier();
    _main2FaderStage = new QAmplifier();


    _subgroup1FaderStage->setGain(0);
    _subgroup2FaderStage->setGain(0);
    _subgroup3FaderStage->setGain(0);
    _subgroup4FaderStage->setGain(0);
    _subgroup5FaderStage->setGain(0);
    _subgroup6FaderStage->setGain(0);
    _subgroup7FaderStage->setGain(0);
    _subgroup8FaderStage->setGain(0);

    _main1FaderStage->setGain(0);
    _main2FaderStage->setGain(0);
}

Mixer::~Mixer()
{

}

void Mixer::process()
{
    // Obtaining sample buffers
    QSampleBuffer subgroup1SampleBuffer = _subGroup1Out->sampleBuffer();
    QSampleBuffer subgroup2SampleBuffer = _subGroup2Out->sampleBuffer();
    QSampleBuffer subgroup3SampleBuffer = _subGroup3Out->sampleBuffer();
    QSampleBuffer subgroup4SampleBuffer = _subGroup4Out->sampleBuffer();
    QSampleBuffer subgroup5SampleBuffer = _subGroup5Out->sampleBuffer();
    QSampleBuffer subgroup6SampleBuffer = _subGroup6Out->sampleBuffer();
    QSampleBuffer subgroup7SampleBuffer = _subGroup7Out->sampleBuffer();
    QSampleBuffer subgroup8SampleBuffer = _subGroup8Out->sampleBuffer();

    QSampleBuffer main1SampleBuffer = _mainLeftOut->sampleBuffer();
    QSampleBuffer main2SampleBuffer = _mainRightOut->sampleBuffer();

    // Clearing buffers (since we are going to sum up signals
    subgroup1SampleBuffer.clear();
    subgroup2SampleBuffer.clear();
    subgroup3SampleBuffer.clear();
    subgroup4SampleBuffer.clear();
    subgroup5SampleBuffer.clear();
    subgroup6SampleBuffer.clear();
    subgroup7SampleBuffer.clear();
    subgroup8SampleBuffer.clear();

    main1SampleBuffer.clear();
    main2SampleBuffer.clear();

    // Find out whether any of the channels is soloed
    bool soloActive = false;
    foreach(Channel *channel, _registeredChannels) {
        if(channel->isSoloed()) {
            soloActive = true;
        }
    }

    // Find out whether any of the subgroups are soloed
    bool subgroupSoloActive = false;
    if(subgroup1Solo
    || subgroup2Solo
    || subgroup3Solo
    || subgroup4Solo
    || subgroup5Solo
    || subgroup6Solo
    || subgroup7Solo
    || subgroup8Solo) {
        subgroupSoloActive = true;
    }

    // Routing channels to subgroups and main
    foreach(Channel *channel, _registeredChannels) {
        // Create a temporary memory buffer, so we do not alter the sample in the input buffer,
        // which may effect other applications connected to the same input.
        QSampleBuffer sampleBuffer = QSampleBuffer::createMemoryAudioBuffer(QJackClient::instance()->bufferSize());

        // Do the processing for the channel
        channel->process(sampleBuffer);

        // If the channel is not muted, apply to subgroups and main.
        if(!channel->isMuted() && (!soloActive || channel->isSoloed())) {
            double panorama = channel->panorama();
            if(channel->isInSubGroup12()) {
                sampleBuffer.addTo(subgroup1SampleBuffer, 1.0 - panorama);
                sampleBuffer.addTo(subgroup2SampleBuffer,       panorama);
            }

            if(channel->isInSubGroup34()) {
                sampleBuffer.addTo(subgroup3SampleBuffer, 1.0 - panorama);
                sampleBuffer.addTo(subgroup4SampleBuffer,       panorama);
            }

            if(channel->isInSubGroup56()) {
                sampleBuffer.addTo(subgroup5SampleBuffer, 1.0 - panorama);
                sampleBuffer.addTo(subgroup6SampleBuffer,       panorama);
            }

            if(channel->isInSubGroup78()) {
                sampleBuffer.addTo(subgroup7SampleBuffer, 1.0 - panorama);
                sampleBuffer.addTo(subgroup8SampleBuffer,       panorama);
            }

            if(channel->isOnMain()) {
                sampleBuffer.addTo(main1SampleBuffer, 1.0 - panorama);
                sampleBuffer.addTo(main2SampleBuffer,       panorama);
            }
        }

        sampleBuffer.releaseMemoryBuffer();
    }

    // Route subgroups through faders
    _subgroup1FaderStage->process(subgroup1SampleBuffer);
    _subgroup2FaderStage->process(subgroup2SampleBuffer);
    _subgroup3FaderStage->process(subgroup3SampleBuffer);
    _subgroup4FaderStage->process(subgroup4SampleBuffer);
    _subgroup5FaderStage->process(subgroup5SampleBuffer);
    _subgroup6FaderStage->process(subgroup6SampleBuffer);
    _subgroup7FaderStage->process(subgroup7SampleBuffer);
    _subgroup8FaderStage->process(subgroup8SampleBuffer);

    // Routing subgroups to main

    if(!subgroup1Mute && subgroup1Main && (!subgroupSoloActive || subgroup1Solo)) {
        subgroup1SampleBuffer.addTo(main1SampleBuffer);
    }
    if(!subgroup2Mute && subgroup2Main && (!subgroupSoloActive || subgroup2Solo)) {
        subgroup2SampleBuffer.addTo(main2SampleBuffer);
    }
    if(!subgroup3Mute && subgroup3Main && (!subgroupSoloActive || subgroup3Solo)) {
        subgroup3SampleBuffer.addTo(main1SampleBuffer);
    }
    if(!subgroup4Mute && subgroup4Main && (!subgroupSoloActive || subgroup4Solo)) {
        subgroup4SampleBuffer.addTo(main2SampleBuffer);
    }
    if(!subgroup5Mute && subgroup5Main && (!subgroupSoloActive || subgroup5Solo)) {
        subgroup5SampleBuffer.addTo(main1SampleBuffer);
    }
    if(!subgroup6Mute && subgroup6Main && (!subgroupSoloActive || subgroup6Solo)) {
        subgroup6SampleBuffer.addTo(main2SampleBuffer);
    }
    if(!subgroup7Mute && subgroup7Main && (!subgroupSoloActive || subgroup7Solo)) {
        subgroup7SampleBuffer.addTo(main1SampleBuffer);
    }
    if(!subgroup8Mute && subgroup8Main && (!subgroupSoloActive || subgroup8Solo)) {
        subgroup8SampleBuffer.addTo(main2SampleBuffer);
    }

    // Peak detection
    _subgroupPeak1 = QUnits::linearToDb(subgroup1SampleBuffer.peak());
    _subgroupPeak2 = QUnits::linearToDb(subgroup2SampleBuffer.peak());
    _subgroupPeak3 = QUnits::linearToDb(subgroup3SampleBuffer.peak());
    _subgroupPeak4 = QUnits::linearToDb(subgroup4SampleBuffer.peak());
    _subgroupPeak5 = QUnits::linearToDb(subgroup5SampleBuffer.peak());
    _subgroupPeak6 = QUnits::linearToDb(subgroup6SampleBuffer.peak());
    _subgroupPeak7 = QUnits::linearToDb(subgroup7SampleBuffer.peak());
    _subgroupPeak8 = QUnits::linearToDb(subgroup8SampleBuffer.peak());

    // Check if main is muted, and clear signal if necessary
    if(main1Mute) {
        main1SampleBuffer.clear();
    } else {
        _main1FaderStage->process(main1SampleBuffer);
    }

    if(main2Mute) {
        main2SampleBuffer.clear();
    } else {
        _main2FaderStage->process(main2SampleBuffer);
    }

    _mainPeak1 = QUnits::linearToDb(main1SampleBuffer.peak());
    _mainPeak2 = QUnits::linearToDb(main2SampleBuffer.peak());
}

void Mixer::registerChannel(int i, Channel *channel)
{
    _registeredChannels.insert(i, channel);
}

void Mixer::resetControls()
{
    subgroup1VolumeVerticalSlider = 0;
    subgroup2VolumeVerticalSlider = 0;
    subgroup3VolumeVerticalSlider = 0;
    subgroup4VolumeVerticalSlider = 0;
    subgroup5VolumeVerticalSlider = 0;
    subgroup6VolumeVerticalSlider = 0;
    subgroup7VolumeVerticalSlider = 0;
    subgroup8VolumeVerticalSlider = 0;

    main1VolumeVerticalSlider = 0;
    main2VolumeVerticalSlider = 0;

    subgroup1Mute = false;
    subgroup2Mute = false;
    subgroup3Mute = false;
    subgroup4Mute = false;
    subgroup5Mute = false;
    subgroup6Mute = false;
    subgroup7Mute = false;
    subgroup8Mute = false;

    main1Mute = false;
    main2Mute = false;

    subgroup1Solo = false;
    subgroup2Solo = false;
    subgroup3Solo = false;
    subgroup4Solo = false;
    subgroup5Solo = false;
    subgroup6Solo = false;
    subgroup7Solo = false;
    subgroup8Solo = false;

    subgroup1Main = true;
    subgroup2Main = true;
    subgroup3Main = true;
    subgroup4Main = true;
    subgroup5Main = true;
    subgroup6Main = true;
    subgroup7Main = true;
    subgroup8Main = true;

    foreach(Channel *channel, _registeredChannels) {
        //FIXIT
        //channel->resetControls();
    }
}
