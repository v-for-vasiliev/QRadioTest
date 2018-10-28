#include "phononplayer.h"

PhononPlayer::PhononPlayer(QObject *parent) :
    QObject(parent)
{
    audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    mediaObject = new Phonon::MediaObject(this);
    Phonon::createPath(mediaObject, audioOutput);
    dummy = new QWidget();
    dummy->setDisabled(true);
    videoWidget =  new Phonon::VideoWidget(dummy);
    Phonon::createPath(mediaObject, videoWidget);
    audioOutput->setVolume(25.0f/100.0f);
}

void PhononPlayer::SetUrl(const QUrl &url)
{
    source = Phonon::MediaSource(url);
}

void PhononPlayer::playNow()
{
    mediaObject->stop();
    mediaObject->clearQueue();
    mediaObject->setCurrentSource(source);
    mediaObject->play();
}

void PhononPlayer::stopNow()
{
    mediaObject->stop();
    mediaObject->clearQueue();
}

void PhononPlayer::setVolume(int value)
{
    if (audioOutput)
        audioOutput->setVolume((qreal)value/100.0f);
}
