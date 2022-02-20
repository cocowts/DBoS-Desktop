#ifndef RECORDERWIDGET_H
#define RECORDERWIDGET_H

#include <QAudioRecorder>
#include <QNetworkAccessManager>
#include <QDialog>
#include <QLabel>
#include <QTimer>

/*
 * 使用百度 api, 请到百度平台申请个人key后进行替换
 * apiKey 和 secretKey 需要替换
 */

class RecorderWidget : public QDialog
{
    Q_OBJECT

    const QString apiKey    = QString("to8Qk2WDAhtpLSCugzEFzp4g");
    const QString secretKey = QString("FRGvqxdMlj9ZRRo6nrNFe4y4sKDWc8RA");

    const QString tokenUrl  = QString("https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&client_id=%1&client_secret=%2&").arg(apiKey).arg(secretKey);

    QString serverApiUrl    = "http://vop.baidu.com/server_api?dev_pid=1537&cuid=%1&token=%2";
    QString accessToken;

public:
    explicit RecorderWidget(QWidget *parent = nullptr);
    ~RecorderWidget();

protected slots:
    void timerTimeout();

protected:
    void mousePressEvent(QMouseEvent *event) override;

protected slots:
    void readyRead();

private:
    void initUi();
    void initCtrl();
    void initAudio();
    void initNet();

    void requestNetwork(const QString &url, const QByteArray &requestData);
    QString getJsonValue(QByteArray data, QString key);
    void audioToText(const QString &path);
    void doSomething(const QString &text);

private:
    QLabel m_movieLbl;
    QLabel m_textLbl;
    QLabel m_iconLbl;
    QTimer m_timer;

    QAudioRecorder m_audioRecorder;

    QNetworkAccessManager m_networkAccessManager;
};
#endif // RECORDERWIDGET_H
