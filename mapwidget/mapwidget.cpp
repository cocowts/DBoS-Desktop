#include "mapwidget.h"

#include "simplemessagebox/simplemessagebox.h"
#include "commonhelper.h"

#include <QSslConfiguration>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QVBoxLayout>
#include <QPixmap>
#include <QSerialPortInfo>

MapWidget::MapWidget(QWidget *parent) : QDialog(parent)
{
    initUi();
    initCtrl();
    setModal(true);
}

MapWidget::~MapWidget()
{
    if (m_port.isOpen())
        m_port.close();
}

void MapWidget::initUi()
{
    auto pMovie = new QMovie(this);
    pMovie->setFileName(":/misc/mapwidget/images/loading.gif");
    pMovie->start();

    m_logoLabel.setObjectName("logoLabel");
    m_logoLabel.setText(QString("当前展示定位坐标：[") +m_location + "]");

    m_iconLabel.setMovie(pMovie);
    m_iconLabel.setAlignment(Qt::AlignCenter);
    m_logoLabel.setAlignment(Qt::AlignRight);

    auto *pVLayout = new QVBoxLayout;
    pVLayout->addStretch();
    pVLayout->addWidget(&m_logoLabel);
    m_iconLabel.setLayout(pVLayout);

    auto *pMainLayout = new QVBoxLayout;
    pMainLayout->addWidget(&m_iconLabel);
    pMainLayout->setMargin(0);
    setLayout(pMainLayout);
    setObjectName("map_widget");
    CommonHelper::setStyleSheet(":/misc/mapwidget/style/default.qss", this);
}

void MapWidget::initCtrl()
{
    initSerialPort();

    requestMapImage(m_location);
}

void MapWidget::initSerialPort()
{
    m_port.setPortName("ttymxc5");

    if(!m_port.open(QIODevice::ReadWrite)) {
        SimpleMessageBox::infomationMessageBox("未检测到设备，请重试");
        m_logoLabel.setText(QString("未检测到GPS设备，当前展示定位坐标：[") +m_location + "]");
        return;
    }

    m_port.setBaudRate(QSerialPort::Baud9600, QSerialPort::AllDirections);
    m_port.setDataBits(QSerialPort::Data8);
    m_port.setFlowControl(QSerialPort::NoFlowControl);
    m_port.setParity(QSerialPort::NoParity);
    m_port.setStopBits(QSerialPort::OneStop);

    connect(&m_port, &QSerialPort::readyRead, this, &MapWidget::portReadReady);
}

void MapWidget::requestMapImage(const QString &center)
{
    QString url = baidudUrl.arg(baidudAk).arg(center);

    m_array.clear();
    requestNetwork(url);
}

void MapWidget::requestNetwork(const QString &url)
{
    QSslConfiguration config;
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    config.setProtocol(QSsl::TlsV1_0);

    QNetworkRequest networkRequest;
    networkRequest.setSslConfiguration(config);
    networkRequest.setUrl(url);

    QNetworkReply *newReply = m_networkAccessManager.get(networkRequest);

    connect(newReply, &QNetworkReply::readyRead, this, &MapWidget::netReadyRead);
    connect(newReply, &QNetworkReply::finished, newReply, &QNetworkReply::deleteLater);
}

void MapWidget::netReadyRead()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    m_array.append(reply->readAll());

    QPixmap pixmap(m_array);

    if(pixmap.loadFromData(m_array)) {
        m_iconLabel.setPixmap(pixmap);
    }
}

void MapWidget::portReadReady()
{
    while (m_port.bytesAvailable()) {
        QString line = m_port.readLine();
        auto location = parseGpsData(line);
        if (!location.isEmpty()) {
            m_location = location;
            line = "定位成功，[" + location + "]";
            requestMapImage(location);
        }
        else {
            line = ("定位中... 请尝试移到空旷位置，展示坐标 [" + m_location + "]");
        }

        m_logoLabel.setText(line);
    }
}

QString MapWidget::parseGpsData(const QString &data)
{
    QString ret;

    if (data.contains(",,,,,"))
        return ret;

    auto list = data.trimmed().split(',');
    if (list.count() != 15)
        return ret;

    if (!list.at(0).contains("$GPGGA"))
        return ret;

    bool ok;
    auto e = list.at(4).toDouble(&ok);
    if (!ok)
        return ret;

    auto n = list.at(2).toDouble(&ok);
    if (!ok)
        return ret;

    int i = e / 100;
    double j = (e / 100  - i) * 100 / 60.0;
    ret = QString::number(i+j, 'f', 6);

    i = n / 100;
    j = (n / 100  - i) * 100 / 60.0;

    ret = ret + ',' + QString::number(i+j, 'f', 6);

    return ret;
}
