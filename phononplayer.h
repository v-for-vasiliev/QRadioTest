#ifndef PHONONPLAYER_H
#define PHONONPLAYER_H

#include <QObject>
#include "defines.h"

class PhononPlayer : public QObject
{
    Q_OBJECT
public:
    explicit PhononPlayer(QObject *parent = 0);
    Phonon::MediaObject* mediaObject;
    void SetUrl(const QUrl &url);
    void playNow();
    void stopNow();
    void setVolume(int value);

signals:

public slots:

private:
    Phonon::AudioOutput* audioOutput;
    Phonon::MediaSource source;
    Phonon::VideoWidget* videoWidget;
    QString url;
    QWidget* dummy;

};

#endif // PHONONPLAYER_H
