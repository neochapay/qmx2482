#ifndef CHANNEL_H
#define CHANNEL_H

#include <QObject>
#include <QSampleBuffer>
#include <QJackPort>
#include <QAmplifier>
#include <QEqualizer>
#include <QJsonObject>

class Channel : public QObject
{
    Q_OBJECT
public:
    explicit Channel(int channelNumber, QObject *parent = 0);
    void process(QSampleBuffer targetSampleBuffer);
    /** Update all visual interface elements. */
    void setPeak(double value);
    double getPeak();

    void setAuxState(bool state);
    bool getAuxState();

    void setEqualizerState(bool state);
    bool getEqualizerState();

    /** @returns the panorama setting. 0.0 means left-most, 1.0 indicates right-most position. */
    double panorama();

    /** @returns whether this channel is contained in subgroup 1 and 2. */
    bool isInSubGroup(QString groupName);

    /** @returns whether this channel has been muted. */
    bool isMuted();
    void setMutedState(bool state);

    /** @returns whether this channel has been soloed. */
    bool isSoloed();
    void setSoledState(bool state);

    /** @returns true, when this channel is routed on main. */
    bool isOnMain();
    void setOnMainState(bool state);


    /** Transfers the current channel state into a JSON object. */
    QJsonObject stateToJson();

    /** Recalls the state from a JSON object. */
    void stateFromJson(QJsonObject jsonObject);

    /** Resets all controls to their default positions. */
    void resetControls();
signals:

public slots:

private:
    /** Input stage amplifier ("Gain"). */
    QAmplifier *_inputStage;
    /** Fader stage amplifier ("Volume"). */
    QAmplifier *_faderStage;
    /** Attenuation stage before sending signal to aux. */
    QAmplifier *_auxPre;
    /** Attenuation stage after receiving signal from aux. */
    QAmplifier *_auxPost;
    /** Equalizer for this channel. */
    QEqualizer *_equalizer;

    /** EQ control for low frequencies. */
    QEqualizerControl *_lowsEqControl;
    /** EQ control for mid frequencies. */
    QEqualizerControl *_midsEqControl;
    /** EQ control for high frequencies. */
    QEqualizerControl *_highsEqControl;

    /** QJackAudio input port for this channel. */
    QJackPort *_channelIn;
    /** QJackAudio aux send output port for this channel. */
    QJackPort *_auxSend;
    /** QJackAudio aux receive input port for this channel. */
    QJackPort *_auxReturn;
    /** QJackAudio direct out output port for this channel. */
    QJackPort *_channelOut;

    /** Last peak value. */
    double _peakDb;

   //Controls variable
    bool _equalizerOn;
    bool _auxOn;
    bool _muteState;
    bool _soloState;
    bool _mainState;

    double _panDial;
};

#endif // CHANNEL_H
