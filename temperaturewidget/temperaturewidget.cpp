#include "temperaturewidget.h"

#include "simplemessagebox/simplemessagebox.h"
#include "commonhelper.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QProcess>

TemperatureWidget::TemperatureWidget(QWidget *parent) : QDialog(parent), m_thread("/dev/mydht11")
{
    initUi();
    initCtrl();
    setModal(true);
}

TemperatureWidget::~TemperatureWidget()
{
    m_thread.quit();
    m_thread.wait();

    QProcess::execute("rmmod /driver/dht11_drv.ko");
}

void TemperatureWidget::initUi()
{
    setObjectName("temperature_widget");

    m_tempLbl.setObjectName("temperature_tempLbl");
    m_tempLbl.setText("当前温度：00 ℃");
    m_tempLbl.setAlignment(Qt::AlignCenter);

    m_humiLbl.setObjectName("temperature_humiLbl");
    m_humiLbl.setText("当前湿度：00 %RH");
    m_humiLbl.setAlignment(Qt::AlignCenter);

    m_tempLine.setTimeAxisXSpanSecs(30);
    m_tempLine.setTimeAxisXFormat(QStringLiteral("mm:ss"));
    m_tempLine.setTimeAxisXTickCount(8);
    m_tempLine.setAxisYRange(-20, 60);
    m_tempLine.setpAxisYTickCount(9);
    m_tempLine.setAxisYLabelFormat(QString::fromLatin1("%.0f℃"));

    QPen pen;
    pen.setWidth(2);
    pen.setColor(QColor(30, 215, 109));
    m_tempLine.addSplineSeries("温度", pen, true);

    m_humiLine.setTimeAxisXSpanSecs(30);
    m_humiLine.setTimeAxisXFormat(QStringLiteral("mm:ss"));
    m_humiLine.setTimeAxisXTickCount(8);
    m_humiLine.setAxisYRange(0, 95);
    m_humiLine.setpAxisYTickCount(9);
    m_humiLine.setAxisYLabelFormat(QString::fromLatin1("%.0f%RH"));
    pen.setColor(QColor(214, 23, 13));
    m_humiLine.addSplineSeries("湿度", pen, true);

    auto pVLayoutLeft = new QVBoxLayout();
    pVLayoutLeft->addWidget(&m_tempLine, 7);
    pVLayoutLeft->addWidget(&m_tempLbl, 1);

    auto pVLayoutRight = new QVBoxLayout();
    pVLayoutRight->addWidget(&m_humiLine, 7);
    pVLayoutRight->addWidget(&m_humiLbl, 1);

    auto pMainLayout = new QHBoxLayout;
    pMainLayout->addLayout(pVLayoutLeft);
    pMainLayout->addLayout(pVLayoutRight);

    setLayout(pMainLayout);
    CommonHelper::setStyleSheet(":/misc/temperaturewidget/style/default.qss", this);
}

void TemperatureWidget::initCtrl()
{
    QProcess::execute("insmod /driver/dht11_drv.ko");

    setValue(0, 0);

    connect(&m_thread, &TemperatureThread::readReady, this, &TemperatureWidget::setValue);

    if (m_thread.start()) {
        m_tempLine.start();
        m_humiLine.start();
    }
    else {
        SimpleMessageBox::infomationMessageBox("未检测到设备，请重试");
    }
}

void TemperatureWidget::setValue(double temp, double humi)
{
    m_tempLbl.setText(QString("当前温度：%1 ℃").arg(temp));
    m_humiLbl.setText(QString("当湿度度：%1 %RH").arg(humi));

    m_tempLine.setSeriesValues(0, temp);
    m_humiLine.setSeriesValues(0, humi);
}

