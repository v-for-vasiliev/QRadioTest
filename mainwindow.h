#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "defines.h"
#include "phononplayer.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void playingStateChanged(Phonon::State newState, Phonon::State /* oldState */);
    void on_playButton_clicked();
    void on_volSlider_valueChanged(int value);
    void on_stopButton_clicked();
    void setBufferingState();
    void on_qualityBox_currentIndexChanged(const QString &arg1);
    void streamProgress(qint64 bytesReceived, qint64 bytesTotal);
    void updateMediaData();

private:
    Ui::MainWindow *ui;
    PhononPlayer *m_Player;
    QUrl url;
    QNetworkAccessManager *networkManager;
    QNetworkRequest *request;
    QNetworkReply *reply;
    int streamMetaint;
    QTimer *metaTimer;
    QEventLoop loop;
};

#endif // MAINWINDOW_H
