#include "ultrasonicwavewidget.h"

#include "commonhelper.h"
#include "simplemessagebox/simplemessagebox.h"

#include <QProcess>

UltrasonicwaveWidget::UltrasonicwaveWidget(QWidget *parent) : QDialog(parent), m_thread("/dev/sr04")
{
    initUi();
    initCtrl();
    setModal(true);
}

UltrasonicwaveWidget::~UltrasonicwaveWidget()
{
    m_thread.quit();
    m_thread.wait();

    QProcess::execute("rmmod /driver/sr04_drv.ko");

    setDistance(0);
}

void UltrasonicwaveWidget::initUi()
{
    m_iconLbl.setObjectName("ultrasonicwave_icon");
    m_iconLbl.setAlignment(Qt::AlignCenter);

    m_statusLbl.setObjectName("ultrasonicwave_state");
    m_statusLbl.setAlignment(Qt::AlignCenter);

    auto pLayout = new QVBoxLayout;

    pLayout->addWidget(&m_iconLbl, 5);
    pLayout->addWidget(&m_statusLbl, 1);

    setLayout(pLayout);
    setObjectName("ultrasonicwave_widget");
    CommonHelper::setStyleSheet(":/misc/ultrasonicwavewidget/style/default.qss", this);
}

void UltrasonicwaveWidget::initCtrl()
{
    QProcess::execute("insmod /driver/sr04_drv.ko");

    setDistance(0);

    if (!m_thread.start()) {
        SimpleMessageBox::infomationMessageBox("未检测到设备，请重试");
    }

    connect(&m_thread, &UltrasonicwaveThread::readReady, this, &UltrasonicwaveWidget::setDistance);
}

void UltrasonicwaveWidget::setDistance(int meter)
{
    m_statusLbl.setText(QString("Distance : %1 Millimeter").arg(meter));
}


