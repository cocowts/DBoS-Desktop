#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QDialog>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QMovie>
#include <QSerialPort>
#include <QByteArray>

class MapWidget : public QDialog
{
    Q_OBJECT

    const QString baidudAk  = "6bsI90pRFA4jcxUMsQ84A2EWyLVgaNcu";
    const QString baidudUrl = "https://api.map.baidu.com/staticimage/v2?ak=%1&mcode=666666&center=%2&width=1024&height=600&zoom=16&scale=1&markers=%2&markerStyles=m,Y,0x00FF00";

public:
    explicit MapWidget(QWidget *parent = nullptr);
    ~MapWidget();

private:
    void initUi();
    void initCtrl();
    void initSerialPort();
    void requestMapImage(const QString &center);
    void requestNetwork(const QString &url);
    QString parseGpsData(const QString &data);

private slots:
    void netReadyRead();
    void portReadReady();

private:
    QLabel m_iconLabel;
    QLabel m_logoLabel;

    QByteArray m_array;
    QNetworkAccessManager m_networkAccessManager;
    QString m_location = QString("118.804341,31.975024");

    QSerialPort m_port;
};

#endif // MAPWIDGET_H
