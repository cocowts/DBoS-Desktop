#include "illuminationwidget.h"

#include "simplemessagebox/simplemessagebox.h"
#include "commonhelper.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QProcess>

IlluminationWidget::IlluminationWidget(QWidget *parent) : QDialog(parent), m_thread("/dev/ap3216c")
{
    initUi();
    initCtrl();
    setModal(true);
}

IlluminationWidget::~IlluminationWidget()
{
    m_thread.quit();
    m_thread.wait();

    QProcess::execute("rmmod /driver/ap3216c_drv.ko");
}

void IlluminationWidget::initUi()
{
    m_irProgressBar.setPieCircleColor(QColor(115, 56, 25));
    m_irProgressBar.setwaterColor(QColor(234, 84, 6));
    m_irProgressBar.setMinValue(0);
    m_irProgressBar.setMaxValue(1023);
    m_irProgressBar.setMinimumHeight(300);
    m_irProgressBar.show();

    m_psProgressBar.setPieCircleColor(QColor(115, 56, 25));
    m_psProgressBar.setwaterColor(QColor(234, 84, 6));
    m_psProgressBar.setMinValue(0);
    m_psProgressBar.setMaxValue(1023);
    m_psProgressBar.setMinimumHeight(300);
    m_psProgressBar.show();

    m_alsProgressBar.setPieCircleColor(QColor(115, 56, 25));
    m_alsProgressBar.setwaterColor(QColor(234, 84, 6));
    m_alsProgressBar.setMinValue(0);
    m_alsProgressBar.setMaxValue(23360);
    m_alsProgressBar.setMinimumHeight(300);
    m_alsProgressBar.show();

    m_irLbl.setObjectName("illumination_ir");
    m_irLbl.setText("红外强度");
    m_irLbl.setAlignment(Qt::AlignCenter);

    m_psLbl.setObjectName("illumination_ps");
    m_psLbl.setText("接近距离");
    m_psLbl.setAlignment(Qt::AlignCenter);

    m_alsLbl.setObjectName("illumination_als");
    m_alsLbl.setText("光照强度");
    m_alsLbl.setAlignment(Qt::AlignCenter);

    auto pVLayout1 = new QVBoxLayout;
    pVLayout1->addWidget(&m_irProgressBar);
    pVLayout1->addWidget(&m_irLbl);
    pVLayout1->setSpacing(0);
    pVLayout1->setMargin(0);

    auto pVLayout2 = new QVBoxLayout;
    pVLayout2->addWidget(&m_psProgressBar);
    pVLayout2->addWidget(&m_psLbl);
    pVLayout2->setSpacing(0);
    pVLayout2->setMargin(0);

    auto pVLayout3 = new QVBoxLayout;
    pVLayout3->addWidget(&m_alsProgressBar);
    pVLayout3->addWidget(&m_alsLbl);
    pVLayout3->setSpacing(0);
    pVLayout3->setMargin(0);

    auto pMainLayout = new QHBoxLayout;
    pMainLayout->addLayout(pVLayout1);
    pMainLayout->addLayout(pVLayout2);
    pMainLayout->addLayout(pVLayout3);
    pMainLayout->setContentsMargins(0, 80, 0, 0);
    pMainLayout->setSpacing(0);

    setLayout(pMainLayout);
    setObjectName("illumination_widget");
    CommonHelper::setStyleSheet(":/misc/illuminationwidget/style/default.qss", this);
}

void IlluminationWidget::initCtrl()
{
    QProcess::execute("insmod /driver/ap3216c_drv.ko");

    setValue(0, 0, 0);

    connect(&m_thread, &IlluminationThread::readReady, this, &IlluminationWidget::setValue);

    if (m_thread.start()) {

    }
    else {
        SimpleMessageBox::infomationMessageBox("未检测到设备，请重试");
    }
}

void IlluminationWidget::setValue(unsigned short ir, unsigned short ps, unsigned short als)
{
    m_irLbl.setText(QString("红外强度 : %1").arg(ir));
    m_psLbl.setText(QString("接近距离 : %1").arg(ps));
    m_alsLbl.setText(QString("光照强度 : %1 LUX").arg(als));

    m_irProgressBar.setValue(ir);
    m_psProgressBar.setValue(ps);
    m_alsProgressBar.setValue(als);
}



