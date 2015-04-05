#ifndef MIXER_H
#define MIXER_H

#include <QTimer>
#include <QAmplifier>
#include <QJackPort>

#include "channel.h"

class Mixer : public QObject
{
    Q_OBJECT
public:
    explicit Mixer(QObject *parent = 0);
    ~Mixer();

    void process();
    void registerChannel(int i, Channel *channel);
    void resetControls();

signals:

public slots:

private:
    /** Update timer used to update the visual interface periodically. */
    QTimer _updateTimer;

    /** Stores all registered channels. */
    QMap<int, Channel*> _registeredChannels;

    /** Used to store calculated peak for subgroup 1. */
    double _subgroupPeak1;
    /** Used to store calculated peak for subgroup 2. */
    double _subgroupPeak2;
    /** Used to store calculated peak for subgroup 3. */
    double _subgroupPeak3;
    /** Used to store calculated peak for subgroup 4. */
    double _subgroupPeak4;
    /** Used to store calculated peak for subgroup 5. */
    double _subgroupPeak5;
    /** Used to store calculated peak for subgroup 6. */
    double _subgroupPeak6;
    /** Used to store calculated peak for subgroup 7. */
    double _subgroupPeak7;
    /** Used to store calculated peak for subgroup 8. */
    double _subgroupPeak8;

    /** Used to store calculated peak for main 1 (left). */
    double _mainPeak1;
    /** Used to store calculated peak for main 2 (right). */
    double _mainPeak2;

    /** Subgroup 1 fader stage. */
    QAmplifier *_subgroup1FaderStage;
    /** Subgroup 2 fader stage. */
    QAmplifier *_subgroup2FaderStage;
    /** Subgroup 3 fader stage. */
    QAmplifier *_subgroup3FaderStage;
    /** Subgroup 4 fader stage. */
    QAmplifier *_subgroup4FaderStage;
    /** Subgroup 5 fader stage. */
    QAmplifier *_subgroup5FaderStage;
    /** Subgroup 6 fader stage. */
    QAmplifier *_subgroup6FaderStage;
    /** Subgroup 7 fader stage. */
    QAmplifier *_subgroup7FaderStage;
    /** Subgroup 8 fader stage. */
    QAmplifier *_subgroup8FaderStage;

    /** Main 1 fader stage. */
    QAmplifier *_main1FaderStage;
    /** Main 2 fader stage. */
    QAmplifier *_main2FaderStage;

    /** Subgroup 1 direct out. */
    QJackPort *_subGroup1Out;
    /** Subgroup 2 direct out. */
    QJackPort *_subGroup2Out;
    /** Subgroup 3 direct out. */
    QJackPort *_subGroup3Out;
    /** Subgroup 4 direct out. */
    QJackPort *_subGroup4Out;
    /** Subgroup 5 direct out. */
    QJackPort *_subGroup5Out;
    /** Subgroup 6 direct out. */
    QJackPort *_subGroup6Out;
    /** Subgroup 7 direct out. */
    QJackPort *_subGroup7Out;
    /** Subgroup 8 direct out. */
    QJackPort *_subGroup8Out;

    /** Main 1 direct out. */
    QJackPort *_mainLeftOut;
    /** Main 2 direct out. */
    QJackPort *_mainRightOut;

    bool main1Mute;
    bool main2Mute;

    bool subgroup1Mute;
    bool subgroup2Mute;
    bool subgroup3Mute;
    bool subgroup4Mute;
    bool subgroup5Mute;
    bool subgroup6Mute;
    bool subgroup7Mute;
    bool subgroup8Mute;

    bool subgroup1Solo;
    bool subgroup2Solo;
    bool subgroup3Solo;
    bool subgroup4Solo;
    bool subgroup5Solo;
    bool subgroup6Solo;
    bool subgroup7Solo;
    bool subgroup8Solo;

    bool subgroup1Main;
    bool subgroup2Main;
    bool subgroup3Main;
    bool subgroup4Main;
    bool subgroup5Main;
    bool subgroup6Main;
    bool subgroup7Main;
    bool subgroup8Main;

    bool subgroupSoloActive;

    double subgroup1VolumeVerticalSlider;
    double subgroup2VolumeVerticalSlider;
    double subgroup3VolumeVerticalSlider;
    double subgroup4VolumeVerticalSlider;
    double subgroup5VolumeVerticalSlider;
    double subgroup6VolumeVerticalSlider;
    double subgroup7VolumeVerticalSlider;
    double subgroup8VolumeVerticalSlider;

    double main1VolumeVerticalSlider;
    double main2VolumeVerticalSlider;

};

#endif // MIXER_H
