#include "electricitywidget.h"

#include "commonhelper.h"
#include "simplemessagebox/simplemessagebox.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSizePolicy>
#include <QProcess>

ElectricityWidget::ElectricityWidget(QWidget *parent) : QDialog(parent)
{
    initUi();
    initCtrl();
    setModal(true);
}

ElectricityWidget::~ElectricityWidget()
{
    if (m_fd != -1) {
        unsigned short value = 0;
        ::write(m_fd, &value, 2);
        ::close(m_fd);
    }

    QProcess::execute("rmmod /driver/dac_drv.ko");
}

void ElectricityWidget::initUi()
{
    m_dashboard.setMinValue(0);
    m_dashboard.setMaxValue(4096);

    m_dashboard.setScaleMajor(9);
    m_dashboard.setScaleMinor(10);

    m_dashboard.setStartAngle(45);
    m_dashboard.setEndAngle(315);

    m_dashboard.setAnimationStepTime(0);

    m_dashboard.setPointerStyle(ColorDashboard::PointerStyle_Triangle);

    m_slider.setObjectName("electricity_slider");
    m_slider.setOrientation(Qt::Horizontal);
    m_slider.setRange(0, 4096);

    m_valueLbl.setObjectName("electricity_valueLbl");
    m_valueLbl.setText("0000 mv");

    auto *pHLayout = new QHBoxLayout;
    pHLayout->addWidget(&m_slider);
    pHLayout->addWidget(&m_valueLbl);
    pHLayout->setMargin(0);
    pHLayout->setSpacing(20);

    auto *pVLayout = new QVBoxLayout;
    pVLayout->addWidget(&m_dashboard, 1);
    pVLayout->addLayout(pHLayout, 0);
    pVLayout->setSpacing(40);
    pVLayout->setContentsMargins(30, 20, 30, 30);

    setLayout(pVLayout);
    setObjectName("electricity_widget");
    CommonHelper::setStyleSheet(":/misc/electricitywidget/style/default.qss", this);
}

void ElectricityWidget::initCtrl()
{
    QProcess::execute("insmod /driver/dac_drv.ko");

    m_fd = ::open("/dev/dac", O_WRONLY);
    if (m_fd < 0) {
        SimpleMessageBox::infomationMessageBox("未检测到设备，请重试");
    }

    connect(&m_slider, &QSlider::valueChanged, this, &ElectricityWidget::valueChanged);
}

void ElectricityWidget::valueChanged(int value)
{
    m_dashboard.setValue(value);
    m_valueLbl.setText(QString("%1 mv").arg(value, 4, 10, QChar('0')));

    if (m_fd != -1) {
        unsigned short t = value / 4;

        if (t > 1023) t = 1023;
        ::write(m_fd, &t, 2);
    }
}


