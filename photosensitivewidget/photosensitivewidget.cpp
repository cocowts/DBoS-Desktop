#include "photosensitivewidget.h"

#include "commonhelper.h"
#include "simplemessagebox/simplemessagebox.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <QVBoxLayout>

PhotosensitiveWidget::PhotosensitiveWidget(QWidget *parent) : QDialog(parent)
{
    initUi();
    initCtrl();
    setModal(true);
}

PhotosensitiveWidget::~PhotosensitiveWidget()
{
    ::close(m_fd);
}

void PhotosensitiveWidget::initUi()
{
    m_arcProgressBar.setArcColor(QColor(247,74,99));
    m_arcProgressBar.setTextColor(QColor(247,74,99));
    m_arcProgressBar.setTitleColor(Qt::white);
    m_arcProgressBar.setAnimationStepTime(10);
    m_arcProgressBar.setTitle("光强");
    m_arcProgressBar.setRange(0, 100);

    auto pVLayout = new QVBoxLayout;
    pVLayout->addWidget(&m_arcProgressBar);
    setLayout(pVLayout);
    setObjectName("photosensitive_widget");
    CommonHelper::setStyleSheet(":/misc/photosensitivewidget/style/default.qss", this);
}

void PhotosensitiveWidget::initCtrl()
{
    m_fd = ::open("/sys/bus/iio/devices/iio:device0/in_voltage3_raw", O_RDONLY);
    if (m_fd != -1) {
        ::close(m_fd);
        connect(&m_timer, &QTimer::timeout, this, &PhotosensitiveWidget::timerTimeout);
        m_timer.start(300);
    }
    else {
        SimpleMessageBox::infomationMessageBox("未检测到设备，请重试");
    }
}

void PhotosensitiveWidget::timerTimeout()
{
    m_fd = ::open("/sys/bus/iio/devices/iio:device0/in_voltage3_raw", O_RDONLY);
    if (m_fd != -1) {
        char data[5] = {0};
        if (::read(m_fd, data, 4) > 0) {
            int val = 100 - 100 * QString(data).toInt() / 4096;
            m_arcProgressBar.setValue(val);
        }
        ::close(m_fd);
    }
}

