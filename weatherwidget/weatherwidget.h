#ifndef WEATHERWIDGET_H
#define WEATHERWIDGET_H

#include <QDialog>
#include <QLabel>
#include <QSplineSeries>
#include <QChart>
#include <QChartView>
#include <QScatterSeries>
#include <QScrollBar>
#include <QScrollArea>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QTimer>

using namespace QtCharts;

/*
 * 使用百度 api, 请到百度平台申请个人key后进行替换
 * userKey 需要替换
 */

class WeatherWidget : public QDialog
{
    Q_OBJECT

    const QString locationName     = QString("南京");
    const QString locationID       = QString("101190101");
    const QString userKey          = QString("16a876594a994d3da5a103f8f1f8daca");

    const QString weatherNowUrl    = QString("https://devapi.qweather.com/v7/weather/now?location=%1&key=%2").arg(locationID).arg(userKey);
    const QString weather7DayUrl   = QString("https://devapi.qweather.com/v7/weather/7d?location=%1&key=%2").arg(locationID).arg(userKey);
    const QString weather24HourUrl = QString("https://devapi.qweather.com/v7/weather/24h?location=%1&key=%2").arg(locationID).arg(userKey);
    const QString airNowUrl        = QString("https://devapi.qweather.com/v7/indices/1d?type=5,8,10&location=%1&key=%2").arg(locationID).arg(userKey);

public:
    explicit WeatherWidget(QWidget *parent = nullptr);

private:
    void initUi();
    void initCtrl();

    QWidget *initCurrentWeatherUi();
    QWidget *initHoursWeatherUi();
    QWidget *initDayWeatherUi();

private slots:
    void timerTimeout();
    void weatherNowReplyFinished(QNetworkReply *reply);
    void weather7DayReplyFinished(QNetworkReply *reply);
    void weather24HourReplyFinished(QNetworkReply *reply);
    void airNowReplyFinished(QNetworkReply *reply);

private:
    QLabel m_listUpdetimeLbl;    // 最近更新时间
    QLabel m_airQualityLbl;      // 空气质量指数
    QLabel m_iconLbl;            // 天气状况图标
    QLabel m_tempLbl;            // 温度
    QLabel m_textLbl;            // 天气状况的文字描述
    QLabel m_dailyTextLbl;       // 生活指数预报的详细描述
    QLabel m_windScaleLbl;       // 风力等级
    QLabel m_windDirLbl;         // 风向
    QLabel m_humidityValueLbl;   // 相对湿度
    QLabel m_humidityLbl;        // 相对湿度
    QLabel m_ultravioletValueLbl;// 紫外线
    QLabel m_ultravioletLbl;     // 紫外线
    QLabel m_feelsLikeValueLbl;  // 体感温度
    QLabel m_feelsLikeLbl;       // 体感温度
    QLabel m_visValueLbl;        // 能见度
    QLabel m_visLbl;             // 能见度
    QLabel m_precipValueLbl;     // 降水量
    QLabel m_precipLbl;          // 降水量
    QLabel m_pressurepValueLbl;  // 大气压
    QLabel m_pressurepLbl;       // 大气压

    QChart m_chart;

    QScatterSeries m_hoursScatterSeries;
    QSplineSeries m_hoursLine;

    QLabel m_hoursWeatherIcon[8];
    QLabel m_hoursWeatherTime[8];

    QProgressBar m_dayTempProgressBar[7];
    QLabel m_dayTemp[7];
    QLabel m_dayIcon[7];
    QLabel m_dayText[7];
    QLabel m_dayDate[7];

    QNetworkAccessManager m_weatherNowNetworkAccessManager;
    QNetworkAccessManager m_weather7DayNetworkAccessManager;
    QNetworkAccessManager m_weather24HourNetworkAccessManager;
    QNetworkAccessManager m_airNowNetworkAccessManager;

    QNetworkRequest m_weatherNowNetWorkRequest;
    QNetworkRequest m_weather7DayNetWorkRequest;
    QNetworkRequest m_weather24HourNetWorkRequest;
    QNetworkRequest m_airNowNetWorkRequest;

    QTimer m_timer;
};

#endif // WEATHERWIDGET_H
