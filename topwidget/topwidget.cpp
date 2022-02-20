#include "topwidget.h"

#include "commonhelper.h"

#include <QHBoxLayout>

TopWidget::TopWidget(QWidget *parent) : QWidget(parent)
{
    m_pCpuInfoLbl->setText("100% 35.62℃");
    m_pCpuInfoLbl->setObjectName("cpuInfoLbl");

    m_pSysTimeLbl->setText("2021-10-11 10:02:57");
    m_pSysTimeLbl->setObjectName("sysTimeLbl");

    QLabel *pCpuLbl = new QLabel(this);
    pCpuLbl->setObjectName("cpuLbl");

    QLabel *pDotsLbl = new QLabel();
    pDotsLbl->setObjectName("dotsLbl");

    QLabel *pVibrateLbl = new QLabel(this);
    pVibrateLbl->setObjectName("vibrateLbl");

    QLabel *pCellSignalXLbl = new QLabel(this);
    pCellSignalXLbl->setObjectName("cellSignalXLbl");

    QLabel *pWifiLbl = new QLabel(this);
    pWifiLbl->setObjectName("wifiLbl");

    QLabel *pbatteryLbl = new QLabel(this);
    pbatteryLbl->setObjectName("batteryLbl");

    auto *pLayout = new QHBoxLayout;
    pLayout->addSpacing(12);
    pLayout->addWidget(pCpuLbl);
    pLayout->addWidget(m_pCpuInfoLbl);
    pLayout->addStretch();
    pLayout->addWidget(m_pSysTimeLbl);
    pLayout->addStretch();
    pLayout->addWidget(pDotsLbl);
    pLayout->addWidget(pVibrateLbl);
    pLayout->addWidget(pCellSignalXLbl);
    pLayout->addWidget(pWifiLbl);
    pLayout->addWidget(pbatteryLbl);
    pLayout->addSpacing(12);
    pLayout->setContentsMargins(0, 8, 0, 8);
    setLayout(pLayout);
    setObjectName("mainWidget");
    CommonHelper::setStyleSheet(":/misc/topwidget/style/default.qss", this);

    setFixedHeight(50);
}

void TopWidget::setCpuInfoText(const QString &text)
{
    m_pCpuInfoLbl->setText(text);
}

void TopWidget::setSysTimeText(const QString &text)
{
    m_pSysTimeLbl->setText(text);
}
