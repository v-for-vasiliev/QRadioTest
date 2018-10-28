#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("HECTAHDART.FM PLAYER");
    //url = QUrl("http://www.nestandartfm.net/HECTAHDART!_FM.m3u");
    url = QUrl("http://95.31.1.103:8000/128");
    this->ui->stopButton->setDisabled(true);
    m_Player = new PhononPlayer(this);
    metaTimer = new QTimer(this);
    metaTimer->setInterval(12000);
    connect(metaTimer, SIGNAL(timeout()), this, SLOT(updateMediaData()));
    streamMetaint = 0;
    connect(m_Player->mediaObject, SIGNAL(stateChanged(Phonon::State,Phonon::State)),this, SLOT(playingStateChanged(Phonon::State,Phonon::State)));
}

MainWindow::~MainWindow()
{
    delete ui;
    if (m_Player)
        m_Player->deleteLater();
}

void MainWindow::playingStateChanged(Phonon::State newState, Phonon::State /* oldState */)
{
    switch (newState) {
    case Phonon::ErrorState:
        this->ui->stateLabel->setText("error");
        if(m_Player->mediaObject){
            if (m_Player->mediaObject->errorType() == Phonon::FatalError) {
                QMessageBox::warning(this, tr("Error:"), m_Player->mediaObject->errorString());
            } else {
                QMessageBox::warning(this, tr("Error"), m_Player->mediaObject->errorString());
            }
        }else{
            QMessageBox::warning(this, tr("Error"),tr("NULL pointer exception"));
        }
        break;

    case Phonon::PlayingState:
        this->ui->stateLabel->setText("playing");
        break;
    case Phonon::StoppedState:
        this->ui->stateLabel->setText("stopped");
        break;
    case Phonon::PausedState:
        this->ui->stateLabel->setText("paused");
        break;
    case Phonon::BufferingState:
        this->ui->stateLabel->setText("buffering...");
        break;
    default:
        break;
    }
}

void MainWindow::on_playButton_clicked()
{
    this->ui->stateLabel->setText("connecting...");
    QTimer::singleShot(1500, this, SLOT(setBufferingState()));
    m_Player->SetUrl(QUrl(QString("http://95.31.1.103:8000/%1").arg(this->ui->qualityBox->currentText())));
    m_Player->playNow();
    this->ui->playButton->setDisabled(true);
    this->ui->stopButton->setDisabled(false);
    metaTimer->start(8000);
}

void MainWindow::on_stopButton_clicked()
{
    m_Player->stopNow();
    this->ui->stateLabel->setText("stopped");
    this->ui->playButton->setDisabled(false);
    this->ui->stopButton->setDisabled(true);
    metaTimer->stop();
    this->ui->titleLabel->setText("N/A");
    this->ui->authorLabel->setText("N/A");
}

void MainWindow::on_volSlider_valueChanged(int value)
{
    m_Player->setVolume(value);
}

void MainWindow::setBufferingState()
{
    this->ui->stateLabel->setText("buffering...");
}

void MainWindow::on_qualityBox_currentIndexChanged(const QString &arg1)
{
    m_Player->stopNow();
    this->ui->stateLabel->setText("connecting...");
    QTimer::singleShot(1500, this, SLOT(setBufferingState()));
    m_Player->SetUrl(QUrl(QString("http://95.31.1.103:8000/%1").arg(arg1)));
    m_Player->playNow();
}

void MainWindow::updateMediaData()
{
    networkManager = new QNetworkAccessManager(this);
    request = new QNetworkRequest(url);
    request->setRawHeader("User-Agent", "Winamp");
    request->setRawHeader("Icy-MetaData", "1");
    reply = networkManager->get(*request);
    connect(reply, SIGNAL(readyRead()), &loop, SLOT(quit()));
    connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(streamProgress(qint64, qint64)));
    loop.exec();
}

void MainWindow::streamProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    if (streamMetaint == 0) {
        streamMetaint = QString(reply->rawHeader("icy-metaint")).toInt();
        qDebug() << QString("Stream MetaInt: %1").arg(streamMetaint);
        if (streamMetaint == 0) return;
    }
    if (bytesReceived > (streamMetaint + 256) && reply->bytesAvailable() > (streamMetaint + 256)) {
        QByteArray tmp(reply->read(reply->bytesAvailable()));
        QString buffer(tmp.mid(streamMetaint + 1 + 13, 256));
        qDebug() << buffer.left(buffer.indexOf("';"));
        QStringList metaData = buffer.split(" - ");
        this->ui->authorLabel->setText(metaData.at(0));
        if (metaData.size() > 3)
            this->ui->titleLabel->setText(metaData.at(1) + metaData.at(2).left(metaData.at(2).length() - 2));
        else
            this->ui->titleLabel->setText(metaData.at(1).left(metaData.at(1).length() - 2));
        streamMetaint = 0;
        disconnect(reply, SIGNAL(readyRead()), &loop, SLOT(quit()));
        disconnect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(streamProgress(qint64, qint64)));
        reply->abort();
        reply->close();
        reply->deleteLater();
        networkManager->deleteLater();
    }
}
