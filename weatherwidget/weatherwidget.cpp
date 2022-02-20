#include "weatherwidget.h"

#include "commonhelper.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScroller>
#include <QTextCodec>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

WeatherWidget::WeatherWidget(QWidget *parent) : QDialog(parent)
{
    initUi();
    initCtrl();
    setModal(true);
}

void WeatherWidget::initUi()
{
    auto pLayout = new QVBoxLayout;
    pLayout->addWidget(initCurrentWeatherUi());
    pLayout->addSpacing(10);
    pLayout->addWidget(initHoursWeatherUi());
    pLayout->addSpacing(10);
    pLayout->addWidget(initDayWeatherUi());
    pLayout->setMargin(10);
    pLayout->setSpacing(0);

    auto centerWidget = new QWidget();
    centerWidget->setObjectName("weather_centerWidget");
    centerWidget->setLayout(pLayout);

    auto pScrollArea = new QScrollArea(this);
    pScrollArea->setObjectName("weather_scrollArea");
    pScrollArea->setAlignment(Qt::AlignCenter);
    pScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    pScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    pScrollArea->setWidget(centerWidget);
    pScrollArea->setWidgetResizable(true);
    auto scroller = QScroller::scroller(pScrollArea);
    scroller->grabGesture(pScrollArea, QScroller::LeftMouseButtonGesture);

    auto pMainLayout = new QHBoxLayout;
    pMainLayout->addWidget(pScrollArea);
    pMainLayout->setMargin(0);
    pMainLayout->setSpacing(0);
    setLayout(pMainLayout);

    setObjectName("weather_widget");
    CommonHelper::setStyleSheet(":/misc/weatherwidget/style/default.qss", this);
}

void WeatherWidget::initCtrl()
{
    QSslConfiguration config;
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    config.setProtocol(QSsl::TlsV1_0);

    m_weatherNowNetWorkRequest.setSslConfiguration(config);
    m_weather7DayNetWorkRequest.setSslConfiguration(config);
    m_weather24HourNetWorkRequest.setSslConfiguration(config);
    m_airNowNetWorkRequest.setSslConfiguration(config);

    m_weatherNowNetWorkRequest.setUrl(weatherNowUrl);
    m_weather7DayNetWorkRequest.setUrl(weather7DayUrl);
    m_weather24HourNetWorkRequest.setUrl(weather24HourUrl);
    m_airNowNetWorkRequest.setUrl(airNowUrl);

    connect(&m_weatherNowNetworkAccessManager, &QNetworkAccessManager::finished, this, &WeatherWidget::weatherNowReplyFinished);
    connect(&m_weather7DayNetworkAccessManager, &QNetworkAccessManager::finished, this, &WeatherWidget::weather7DayReplyFinished);
    connect(&m_weather24HourNetworkAccessManager, &QNetworkAccessManager::finished, this, &WeatherWidget::weather24HourReplyFinished);
    connect(&m_airNowNetworkAccessManager, &QNetworkAccessManager::finished, this, &WeatherWidget::airNowReplyFinished);
    connect(&m_timer, &QTimer::timeout, this, &WeatherWidget::timerTimeout);

    timerTimeout();

    m_timer.start(5 * 60 * 1000);
}

QWidget *WeatherWidget::initCurrentWeatherUi()
{
    auto currentWeatherWidget = new QWidget;
    currentWeatherWidget->setObjectName("weather_currentWeatherWidget");

    m_listUpdetimeLbl.setObjectName("weather_listUpdetimeLbl");
    m_airQualityLbl.setObjectName("weather_airQualityLbl");

    m_listUpdetimeLbl.setAlignment(Qt::AlignRight);
    m_airQualityLbl.setAlignment(Qt::AlignRight);

    m_listUpdetimeLbl.setText("2022-01-01 19:31");
    m_airQualityLbl.setText("AQI 良 - 南京");

    // 天气简要描述 ========================================
    m_iconLbl.setObjectName("weather_iconLbl");
    m_tempLbl.setObjectName("weather_tempLbl");
    m_textLbl.setObjectName("weather_textLbl");

    m_iconLbl.setFixedSize(180, 180);
    m_iconLbl.setScaledContents(true);
    m_iconLbl.setAlignment(Qt::AlignRight);
    m_iconLbl.setPixmap(QPixmap(":/misc/weatherwidget/images/colorful/300.png"));
    m_tempLbl.setText("5°");
    m_textLbl.setText("晴");

    auto pVboxLayout = new QVBoxLayout;
    pVboxLayout->addWidget(&m_tempLbl);
    pVboxLayout->addWidget(&m_textLbl);
    pVboxLayout->addStretch();
    pVboxLayout->setContentsMargins(0, 35, 0, 0);
    pVboxLayout->setSpacing(0);

    auto pHboxLayout = new QHBoxLayout;
    pHboxLayout->addStretch();
    pHboxLayout->addWidget(&m_iconLbl);
    pHboxLayout->addLayout(pVboxLayout);
    pHboxLayout->addStretch();
    pHboxLayout->setSpacing(10);
    pHboxLayout->setMargin(0);

    // 天气的文字描述 ========================================
    m_dailyTextLbl.setObjectName("weather_text");
    m_dailyTextLbl.setAlignment(Qt::AlignCenter);
    m_dailyTextLbl.setText("白天天气晴好，但仍会使您感觉偏冷，不舒适，请注意适时添加衣物，以防感冒。");

    //  详细 ========================================
    m_windScaleLbl.setObjectName("weather_windScaleLbl");
    m_windDirLbl.setObjectName("weather_windDirLbl");
    m_humidityValueLbl.setObjectName("weather_humidityValueLbl");
    m_humidityLbl.setObjectName("weather_humidityLbl");
    m_ultravioletValueLbl.setObjectName("weather_ultravioletValueLbl");
    m_ultravioletLbl.setObjectName("weather_ultravioletLbl");
    m_feelsLikeValueLbl.setObjectName("weather_feelsLikeValueLbl");
    m_feelsLikeLbl.setObjectName("weather_feelsLikeLbl");
    m_visValueLbl.setObjectName("weather_visValueLbl");
    m_visLbl.setObjectName("weather_visLbl");
    m_precipValueLbl.setObjectName("weather_precipValueLbl");
    m_precipLbl.setObjectName("weather_precipLbl");
    m_pressurepValueLbl.setObjectName("weather_pressurepValueLbl");
    m_pressurepLbl.setObjectName("weather_pressurepLbl");

    m_windScaleLbl.setText("4级");
    m_windDirLbl.setText("东北风");
    m_humidityValueLbl.setText("36%");
    m_humidityLbl.setText("相对湿度");
    m_ultravioletValueLbl.setText("弱");
    m_ultravioletLbl.setText("紫外线");
    m_feelsLikeValueLbl.setText("2°");
    m_feelsLikeLbl.setText("体感温度");
    m_visValueLbl.setText("12km");
    m_visLbl.setText("能见度");
    m_precipValueLbl.setText("0.00mm");
    m_precipLbl.setText("降水量");
    m_pressurepValueLbl.setText("1012hPa");
    m_pressurepLbl.setText("大气压");

    auto pDetailedWidget = new QWidget;
    pDetailedWidget->setObjectName("weather_detailedWidget");

    auto detailedWidgetLayout = new QGridLayout;
    detailedWidgetLayout->addWidget(&m_windScaleLbl,        0, 0, Qt::AlignCenter);
    detailedWidgetLayout->addWidget(&m_humidityValueLbl,    0, 1, Qt::AlignCenter);
    detailedWidgetLayout->addWidget(&m_ultravioletValueLbl, 0, 2, Qt::AlignCenter);
    detailedWidgetLayout->addWidget(&m_feelsLikeValueLbl,   0, 3, Qt::AlignCenter);
    detailedWidgetLayout->addWidget(&m_visValueLbl,         0, 4, Qt::AlignCenter);
    detailedWidgetLayout->addWidget(&m_precipValueLbl,      0, 5, Qt::AlignCenter);
    detailedWidgetLayout->addWidget(&m_pressurepValueLbl,   0, 6, Qt::AlignCenter);

    detailedWidgetLayout->addWidget(&m_windDirLbl,          1, 0, Qt::AlignCenter);
    detailedWidgetLayout->addWidget(&m_humidityLbl,         1, 1, Qt::AlignCenter);
    detailedWidgetLayout->addWidget(&m_ultravioletLbl,      1, 2, Qt::AlignCenter);
    detailedWidgetLayout->addWidget(&m_feelsLikeLbl,        1, 3, Qt::AlignCenter);
    detailedWidgetLayout->addWidget(&m_visLbl,              1, 4, Qt::AlignCenter);
    detailedWidgetLayout->addWidget(&m_precipLbl,           1, 5, Qt::AlignCenter);
    detailedWidgetLayout->addWidget(&m_pressurepLbl,        1, 6, Qt::AlignCenter);
    detailedWidgetLayout->setVerticalSpacing(0);

    pDetailedWidget->setLayout(detailedWidgetLayout);

    // 整体布局
    auto pMainLayout = new QVBoxLayout;

    pMainLayout->addWidget(&m_listUpdetimeLbl, Qt::AlignRight);
    pMainLayout->addWidget(&m_airQualityLbl, Qt::AlignRight);
    pMainLayout->addLayout(pHboxLayout);
    pMainLayout->addSpacing(0);
    pMainLayout->addWidget(&m_dailyTextLbl);
    pMainLayout->addSpacing(30);
    pMainLayout->addWidget(pDetailedWidget);
    pMainLayout->setContentsMargins(20, 10, 20, 10);
    pMainLayout->setSpacing(0);

    currentWeatherWidget->setLayout(pMainLayout);

    return currentWeatherWidget;
}

QWidget *WeatherWidget::initHoursWeatherUi()
{
    auto *pHourshWeather = new QWidget;
    pHourshWeather->setObjectName("weather_hourshWeather");

    auto pHoursChartView = new QChartView;
    pHoursChartView->setStyleSheet("background: transparent");

    for (int i=0; i<8; ++i) {
        m_hoursLine << QPointF(i, i*4);
        m_hoursScatterSeries << QPointF(i, i*4);
    }

    QFont font;
    font.setPointSize(15);
    m_hoursScatterSeries.setPointLabelsColor(QColor(136, 136, 137));
    m_hoursScatterSeries.setPointLabelsFont(font);
    m_hoursScatterSeries.setPointLabelsFormat("@yPoint°");
    m_hoursScatterSeries.setPointLabelsVisible();
    m_hoursScatterSeries.setMarkerSize(0);

    m_chart.addSeries(&m_hoursScatterSeries);
    m_chart.addSeries(&m_hoursLine);
    m_chart.createDefaultAxes();
    m_chart.axes(Qt::Horizontal).first()->setRange(-0.4, 7.4);
    m_chart.axes(Qt::Vertical).first()->setRange(0, 40);
    m_chart.axes(Qt::Horizontal).first()->setGridLineVisible(false);
    m_chart.axes(Qt::Vertical).first()->setGridLineVisible(false);
    m_chart.axes(Qt::Horizontal).first()->setVisible(false);
    m_chart.axes(Qt::Vertical).first()->setVisible(false);
    m_chart.legend()->setVisible(false);
    m_chart.setBackgroundRoundness(20);
    m_chart.setMargins(QMargins(0, 0, 0, 0));
    m_chart.setContentsMargins(0, 0, 0, 0);
    m_chart.setBackgroundVisible(false);

    pHoursChartView->setChart(&m_chart);
    pHoursChartView->setRenderHint(QPainter::Antialiasing);

    //=================================
    auto pInfoLayout = new QGridLayout;
    pInfoLayout->setMargin(0);

    for (int i=0; i<8; ++i) {
        m_hoursWeatherIcon[i].setObjectName(QString("weather_hoursWeatherIcon%1").arg(i));
        m_hoursWeatherTime[i].setObjectName(QString("weather_hoursWeatherTime%1").arg(i));

        pInfoLayout->addWidget(&m_hoursWeatherIcon[i], 0, i, Qt::AlignCenter);
        pInfoLayout->addWidget(&m_hoursWeatherTime[i], 1, i, Qt::AlignCenter);

        m_hoursWeatherIcon[i].setScaledContents(true);
        m_hoursWeatherIcon[i].setFixedSize(48, 48);
        m_hoursWeatherIcon[i].setPixmap(QPixmap(":/misc/weatherwidget/images/colorful/301.png"));
        m_hoursWeatherTime[i].setText(QString("21:00"));
    }

    auto pVLayout = new QVBoxLayout;
    pVLayout->addWidget(pHoursChartView);
    pVLayout->addLayout(pInfoLayout);
    pHourshWeather->setLayout(pVLayout);
    pVLayout->setContentsMargins(0, 0, 0, 10);
    pVLayout->setSpacing(0);
    pHourshWeather->setFixedHeight(180);

    return pHourshWeather;
}

QWidget *WeatherWidget::initDayWeatherUi()
{
    auto *pDayWeather = new QWidget;
    pDayWeather->setObjectName("weather_dayWeather");

    auto pLayout = new QGridLayout;

    for (int i=0; i<7; ++i) {
        m_dayTempProgressBar[i].setObjectName(QString("weather_dayTempProgressBar%1").arg(i));
        m_dayTemp[i].setObjectName(QString("weather_dayTemp%1").arg(i));
        m_dayText[i].setObjectName(QString("weather_dayText%1").arg(i));
        m_dayDate[i].setObjectName(QString("weather_dayDate%1").arg(i));

        pLayout->addWidget(&m_dayTempProgressBar[i],  1, i, Qt::AlignCenter);
        pLayout->addWidget(&m_dayTemp[i],             2, i, Qt::AlignCenter);
        pLayout->addWidget(&m_dayIcon[i],             3, i, Qt::AlignCenter);
        pLayout->addWidget(&m_dayText[i],             4, i, Qt::AlignCenter);
        pLayout->addWidget(&m_dayDate[i],             5, i, Qt::AlignCenter);

        m_dayTempProgressBar[i].setTextVisible(false);
        m_dayTempProgressBar[i].setRange(-10, 50);
        m_dayTempProgressBar[i].setValue(50);
        m_dayTempProgressBar[i].setOrientation(Qt::Vertical);

        m_dayTemp[i].setText("50° / -3°");
        m_dayIcon[i].setScaledContents(true);
        m_dayIcon[i].setFixedSize(48, 48);
        m_dayIcon[i].setPixmap(QPixmap(":/misc/weatherwidget/images/colorful/103.png"));
        m_dayText[i].setText("晴天");
        m_dayDate[i].setText("01月6日");
    }

    m_dayTempProgressBar[0].setValue(13);
    m_dayTempProgressBar[1].setValue(50);
    m_dayTempProgressBar[2].setValue(12);
    m_dayTempProgressBar[3].setValue(20);
    m_dayTempProgressBar[4].setValue(-3);
    m_dayTempProgressBar[5].setValue(5);
    m_dayTempProgressBar[6].setValue(18);

    pLayout->setSpacing(0);
    pLayout->setContentsMargins(0, 20, 0, 0);
    pDayWeather->setLayout(pLayout);

    return pDayWeather;
}

void WeatherWidget::timerTimeout()
{
    m_weatherNowNetworkAccessManager.get(m_weatherNowNetWorkRequest);

    m_weather7DayNetworkAccessManager.get(m_weather7DayNetWorkRequest);

    m_weather24HourNetworkAccessManager.get(m_weather24HourNetWorkRequest);

    m_airNowNetworkAccessManager.get(m_airNowNetWorkRequest);
}

void WeatherWidget::weatherNowReplyFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError)
        return;

    QJsonDocument jd = QJsonDocument::fromJson(reply->readAll());
    if (jd.isObject()) {
        auto root_obj = jd.object();
        if (root_obj.value("code").toString() != "200") {
            return;
        }

        auto updateTime = root_obj.value("updateTime").toString().remove(-6, 6).replace("T", " ");
        auto weather_Value = root_obj.value("now").toObject();
        auto temp = weather_Value.value("temp").toString();
        auto feelsLike = weather_Value.value("feelsLike").toString();
        auto icon = weather_Value.value("icon").toString();
        auto text = weather_Value.value("text").toString();
        auto windDir = weather_Value.value("windDir").toString();
        auto windScale = weather_Value.value("windScale").toString();
        auto humidity = weather_Value.value("humidity").toString();
        auto precip = weather_Value.value("precip").toString();
        auto pressure = weather_Value.value("pressure").toString();
        auto vis = weather_Value.value("vis").toString();

        m_listUpdetimeLbl.setText(updateTime);
        m_iconLbl.setPixmap(QString(":/misc/weatherwidget/images/colorful/%1.png").arg(icon.toInt()));
        m_tempLbl.setText(temp + "°");
        m_textLbl.setText(text);
        m_windScaleLbl.setText(windScale + "级");
        m_windDirLbl.setText(windDir);
        m_humidityValueLbl.setText(humidity + "%");
        m_feelsLikeValueLbl.setText(feelsLike + "°");
        m_visValueLbl.setText(vis + "km");
        m_precipValueLbl.setText(precip + "mm");
        m_pressurepValueLbl.setText(pressure + "hPa");
    }
}

void WeatherWidget::weather7DayReplyFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        return;
    }

    QJsonDocument jd = QJsonDocument::fromJson(reply->readAll());
    if (jd.isObject()) {
        QJsonObject root_obj = jd.object();

        if (root_obj.value("code").toString() != "200") {
            return;
        }

        int tempMinAll = 99;
        int tempMaxAll = -99;
        auto daily_array = root_obj.value("daily").toArray();
        for (auto i = 0; i < daily_array.size(); ++i) {
            auto obj = daily_array.at(i).toObject();
            auto tempMax = obj.value("tempMax").toString().toInt();
            auto tempMin = obj.value("tempMin").toString().toInt();
            auto iconDay = obj.value("iconDay").toString();
            auto textDay = obj.value("textDay").toString();
            auto fxDate = obj.value("fxDate").toString().mid(5);

            if (tempMax > tempMaxAll)
                tempMaxAll = tempMax;
            if (tempMin < tempMinAll)
                tempMinAll = tempMin;

            m_dayText[i].setText(textDay);
            m_dayDate[i].setText(fxDate);
            m_dayIcon[i].setPixmap(QString(":/misc/weatherwidget/images/colorful/%1.png").arg(iconDay));
            m_dayTemp[i].setText(QString("%1° / %2°").arg(tempMax).arg(tempMin));
            m_dayTempProgressBar[i].setValue(tempMax);
        }

        for (auto i = 0; i < daily_array.size(); ++i) {
            m_dayTempProgressBar[i].setRange(tempMinAll, tempMaxAll);
        }
    }
}

void WeatherWidget::weather24HourReplyFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError)
        return;

    QJsonDocument jd = QJsonDocument::fromJson(reply->readAll());
    if (jd.isObject()) {
        QJsonObject root_obj = jd.object();

        if (root_obj.value("code").toString() != "200") {
            return;
        }

        QList<int> tempList;
        int tempMin = 99;
        int tempMax = -99;
        auto hourly_array = root_obj.value("hourly").toArray();
        for (auto i = 0, j = 0; i < hourly_array.size(); ++i) {
            if (i % 3 == 0) {
                auto obj = hourly_array.at(i).toObject();
                auto fxTime = obj.value("fxTime").toString().mid(11, 5);
                auto icon = obj.value("icon").toString();
                auto temp = obj.value("temp").toString().toInt();

                if (temp > tempMax)
                    tempMax = temp;
                if (temp < tempMin)
                    tempMin = temp;

                m_hoursScatterSeries.replace(j, j, temp);
                m_hoursLine.replace(j, j, temp);
                m_hoursWeatherIcon[j].setPixmap(QString(":/misc/weatherwidget/images/colorful/%1.png").arg(icon));
                m_hoursWeatherTime[j].setText(fxTime);

                ++j;
            }
        }

        m_chart.axes(Qt::Vertical).first()->setRange(tempMin-1, tempMax+4);
    }
}

void WeatherWidget::airNowReplyFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError)
        return;

    QJsonDocument jd = QJsonDocument::fromJson(reply->readAll());
    if (jd.isObject()) {
        auto root_obj = jd.object();
        if (root_obj.value("code").toString() != "200") {
            return;
        }
        auto daily_array = root_obj.value("daily").toArray();
        for (auto i = 0; i < daily_array.size(); ++i) {
            auto obj = daily_array.at(i).toObject();
            if (obj.value("type").toString() == "5") {
                m_ultravioletValueLbl.setText(obj.value("category").toString());
            }
            else if (obj.value("type").toString() == "8") {
                m_dailyTextLbl.setText(obj.value("text").toString());
            }
            else if (obj.value("type").toString() == "10") {
                m_airQualityLbl.setText(QString("AQI ") + obj.value("category").toString() + " - " + locationName);
            }
        }
    }
}
