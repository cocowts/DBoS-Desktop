#include "recorderwidget.h"

#include "commonhelper.h"
#include "simplemessagebox/simplemessagebox.h"

#include <QVBoxLayout>
#include <QMovie>
#include <QUrl>
#include <QSslConfiguration>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QHostInfo>
#include <QFile>

RecorderWidget::RecorderWidget(QWidget *parent) : QDialog(parent)
{
    initUi();
    initCtrl();
    setModal(true);
}

RecorderWidget::~RecorderWidget()
{
    m_audioRecorder.stop();
}

void RecorderWidget::initUi()
{
    auto pMovie = new QMovie(this);
    pMovie->setFileName(":/misc/recorderwidget/images/voice.gif");
    pMovie->jumpToNextFrame();

    m_movieLbl.setMovie(pMovie);
    m_movieLbl.setAlignment(Qt::AlignCenter);
    m_textLbl.setText("点击任意处，开始说话～");
    m_textLbl.setObjectName("recorder_textLbl");
    m_textLbl.setAlignment(Qt::AlignCenter);
    m_iconLbl.setAlignment(Qt::AlignCenter);
    m_iconLbl.setObjectName("recorder_iconLbl");

    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget(&m_movieLbl);
    pLayout->addWidget(&m_textLbl);
    pLayout->addSpacing(20);
    pLayout->addWidget(&m_iconLbl);
    pLayout->setContentsMargins(0, 0, 0, 10);

    setLayout(pLayout);
    setObjectName("recorder_widget");
    CommonHelper::setStyleSheet(":/misc/recorderwidget/style/default.qss", this);
}

void RecorderWidget::initCtrl()
{
    initAudio();
    initNet();

    m_timer.setSingleShot(true);

    connect(&m_timer, &QTimer::timeout, this, &RecorderWidget::timerTimeout);
}

void RecorderWidget::initAudio()
{
    QAudioEncoderSettings settings;
    settings.setSampleRate(16000);
    settings.setBitRate(0);
    settings.setChannelCount(1);
    settings.setEncodingMode(QMultimedia::ConstantQualityEncoding);

    m_audioRecorder.setEncodingSettings(settings, QVideoEncoderSettings(), "audio/x-wav");
    m_audioRecorder.setOutputLocation(QUrl::fromLocalFile(tr("./recorder.wav")));
}

void RecorderWidget::initNet()
{
    requestNetwork(tokenUrl, QByteArray());
}

void RecorderWidget::timerTimeout()
{
    m_audioRecorder.stop();
    m_movieLbl.movie()->jumpToFrame(1);
    m_textLbl.setText("正在识别，请稍后～");
    audioToText("./recorder.wav");
}

void RecorderWidget::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)

    if (m_timer.isActive()) {
        m_timer.stop();
        timerTimeout();
    }
    else {
        m_iconLbl.setText(" ");
        m_movieLbl.movie()->start();
        m_timer.start(6000);
        m_textLbl.setText("请继续说话，点击任意处停止～");
        m_audioRecorder.record();
    }
}

void RecorderWidget::requestNetwork(const QString &url, const QByteArray &requestData)
{
    QSslConfiguration config;
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    config.setProtocol(QSsl::TlsV1_0);

    QNetworkRequest networkRequest;
    networkRequest.setSslConfiguration(config);
    networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json;charset=UTF-8");
    networkRequest.setRawHeader("Content-Type", QString("audio/pcm;rate=16000").toLatin1());
    networkRequest.setUrl(url);

    QNetworkReply *newReply = m_networkAccessManager.post(networkRequest, requestData);

    connect(newReply, &QNetworkReply::readyRead, this, &RecorderWidget::readyRead);
    connect(newReply, &QNetworkReply::finished, newReply, &QNetworkReply::deleteLater);
}

void RecorderWidget::readyRead()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    QByteArray data = reply->readAll();

    if (reply->url() == tokenUrl) {
        accessToken = getJsonValue(data, "access_token");
        serverApiUrl = QString(serverApiUrl.arg(QHostInfo::localHostName()).arg(accessToken));
    }
    else if (reply->url() == serverApiUrl) {
        QString text = getJsonValue(data, "result");
        if (text.isEmpty()) {
            m_textLbl.setText("语音识别失败，请保持语音清晰～");
        }
        else {
            m_textLbl.setText(QString("识别结果： ") + text.remove(text.length()-1, 1));
            doSomething(text);
        }
    }
}

QString RecorderWidget::getJsonValue(QByteArray data, QString key)
{
    QString ret;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(data);
    if (jsonDocument.isObject()) {
        QJsonObject jsonObj = jsonDocument.object();
        if (jsonObj.contains(key)) {
            QJsonValue jsonVal= jsonObj.value(key);
            if (jsonVal.isString()) {
                ret = jsonVal.toString();
            }
            else if (jsonVal.isArray()) {
                QJsonArray arr = jsonVal.toArray();
                QJsonValue jv = arr.at(0);
                ret = jv.toString();
            }
        }
    }
    return ret;
}

void RecorderWidget::audioToText(const QString &path)
{
    QFile file(path);

    if (!file.exists()) {
        m_textLbl.setText("语音识别失败，请重试～");
        return;
    }

    QByteArray requestData;
    if (file.open(QIODevice::ReadOnly)) {
        requestData = file.readAll();
        file.close();
    }

    requestNetwork(serverApiUrl, requestData);
}


void RecorderWidget::doSomething(const QString &text)
{
    if (text.contains("酷"))
        m_iconLbl.setPixmap(QPixmap(":/misc/recorderwidget/images/cool.png"));
    else if (text.contains("哭"))
        m_iconLbl.setPixmap(QPixmap(":/misc/recorderwidget/images/cry.png"));
    else if (text.contains("高兴"))
        m_iconLbl.setPixmap(QPixmap(":/misc/recorderwidget/images/happy.png"));
    else if (text.contains("受伤"))
        m_iconLbl.setPixmap(QPixmap(":/misc/recorderwidget/images/injuried.png"));
    else if (text.contains("恋爱"))
        m_iconLbl.setPixmap(QPixmap(":/misc/recorderwidget/images/inlove.png"));
    else if (text.contains("眼红"))
        m_iconLbl.setPixmap(QPixmap(":/misc/recorderwidget/images/envious.png"));
}


