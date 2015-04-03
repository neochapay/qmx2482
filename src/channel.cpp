#include "channel.h"

Channel::Channel(int channelNumber, QObject *parent) :
    QObject(parent)
{
    QJackClient *jackClient = QJackClient::instance();
    _channelIn  = jackClient->registerAudioInPort (QString("ch%1_in")       .arg(channelNumber));
    _auxSend    = jackClient->registerAudioOutPort(QString("ch%1_aux_send") .arg(channelNumber));
    _auxReturn  = jackClient->registerAudioInPort (QString("ch%1_aux_ret")  .arg(channelNumber));
    _channelOut = jackClient->registerAudioOutPort(QString("ch%1_out")      .arg(channelNumber));


    // Create input and fader stage amplifiers
    _inputStage = new QAmplifier();
    _inputStage->setGain(0);

    _faderStage = new QAmplifier();
    _faderStage->setGain(0);

    // Create aux pre and post amplifiers
    _auxPre = new QAmplifier();
    _auxPre->setGain(0);
    _auxPost = new QAmplifier();
    _auxPost->setGain(0);

    // Create equalizer
    _equalizer = new QEqualizer(256, 128);

    // Create equalizer controls
    _lowsEqControl = _equalizer->createEqualizerControl(QEqualizerControl::LowShelf);
    _lowsEqControl->setAmount(0);
    _lowsEqControl->setControlFrequency(200);
    _lowsEqControl->setQ(1.2);

    _midsEqControl = _equalizer->createEqualizerControl(QEqualizerControl::Band);
    _midsEqControl->setAmount(0);
    _midsEqControl->setControlFrequency(4000);
    _midsEqControl->setBandwidth(500.0);

    _highsEqControl = _equalizer->createEqualizerControl(QEqualizerControl::HighShelf);
    _highsEqControl->setAmount(0);
    _highsEqControl->setControlFrequency(12000);
    _highsEqControl->setQ(0.5);

    _equalizer->update();
}

void Channel::process(QSampleBuffer targetSampleBuffer)
{
    // Get the hardware input buffer for this channel input
    QSampleBuffer inputSampleBuffer = _channelIn->sampleBuffer();

    // Copy all data to a memory buffer
    inputSampleBuffer.copyTo(targetSampleBuffer);

    // Process input stage amplifier
    _inputStage->process(targetSampleBuffer);

    // Check if EQ is activated and process
    if(_equalizerOn) {
        _equalizer->process(targetSampleBuffer);
    }

    // Check if aux send/return is activated and process
    if(_auxOn) {
        // Attenuate signal
        _auxPre->process(targetSampleBuffer);
        // Send signal
        targetSampleBuffer.copyTo(_auxSend->sampleBuffer());
        // Take received signal
        _auxReturn->sampleBuffer().copyTo(targetSampleBuffer);
        // Attenuate signal
        _auxPost->process(targetSampleBuffer);
    }

    // Process fader stage amplifier
    _faderStage->process(targetSampleBuffer);

    // Determine peak and convert to dB.
    _peakDb = QUnits::linearToDb(targetSampleBuffer.peak());

    // Transfer data to channel direct out.
    targetSampleBuffer.copyTo(_channelOut->sampleBuffer());
}


void Channel::setEqualizerState(bool state)
{
    _equalizerOn = state;
}

bool Channel::getEqualizerState()
{
    return _equalizerOn;
}

void Channel::setAuxState(bool state)
{
    _auxOn = state;
}

bool Channel::getAuxState()
{
    return _auxOn;
}

double Channel::panorama()
{
    return _panDial / 100.0;
}

bool Channel::isMuted()
{
    return _muteState;
}

void Channel::setMutedState(bool state)
{
    _muteState = state;
}

bool Channel::isSoloed()
{
    return _soloState;
}

void Channel::setSoledState(bool state)
{
    _soloState = state;
}

bool Channel::isOnMain()
{
    return _mainState;
}

void Channel::setOnMainState(bool state)
{
    _mainState = state;
}

double Channel::getPeak()
{
    return _peakDb;
}

void Channel::setPeak(double value)
{
    _peakDb = value;
}


QJsonObject Channel::stateToJson()
{
    QJsonObject jsonObject;

    jsonObject.insert("inputGain", _inputStage->gain());

    jsonObject.insert("eqActive", _equalizerOn);
    jsonObject.insert("highAmount", _highsEqControl->amount());
    jsonObject.insert("highFrequency", _highsEqControl->controlFrequency());
    jsonObject.insert("midFrequency", _midsEqControl->controlFrequency());
    jsonObject.insert("midAmount", _midsEqControl->amount());
    jsonObject.insert("lowFrequency", _lowsEqControl->controlFrequency());
    jsonObject.insert("lowAmount", _lowsEqControl->amount());

    jsonObject.insert("auxActive", _auxOn);
    jsonObject.insert("auxSendGain", _auxPre->gain());
    jsonObject.insert("auxReturnGain", _auxPost->gain());

    jsonObject.insert("muted", _muteState);
    jsonObject.insert("soloed", _soloState);

    jsonObject.insert("faderGain", _faderStage->gain());

    jsonObject.insert("onMain", _mainState);

    return jsonObject;
}

void Channel::stateFromJson(QJsonObject jsonObject)
{
    _inputStage->setGain(jsonObject.value("inputGain").toDouble());

    setEqualizerState(jsonObject.value("eqActive").toBool());
    _highsEqControl->setAmount(jsonObject.value("highAmount").toDouble());
    _highsEqControl->setControlFrequency(jsonObject.value("hightFrequency").toDouble());
    _midsEqControl->setControlFrequency(jsonObject.value("midFrequency").toDouble());
    _midsEqControl->setAmount(jsonObject.value("midAmount").toDouble());
    _lowsEqControl->setControlFrequency(jsonObject.value("lowFrequency").toDouble());
    _lowsEqControl->setAmount(jsonObject.value("lowAmount").toDouble());

    setAuxState(jsonObject.value("auxActive").toBool());
    _auxPre->setGain(jsonObject.value("auxSendGain").toDouble());
    _auxPost->setGain(jsonObject.value("auxReturnGain").toDouble());

    setMutedState(jsonObject.value("muted").toBool());
    setSoledState(jsonObject.value("soloed").toBool());

    _faderStage->setGain(jsonObject.value("faderGain").toDouble());

    setOnMainState(jsonObject.value("onMain").toBool());
}
