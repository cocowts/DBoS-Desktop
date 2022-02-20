#include "infraredwidget.h"

#include "commonhelper.h"
#include "simplemessagebox/simplemessagebox.h"

#include <QVBoxLayout>
#include <QMovie>
#include <QFile>
#include <QProcess>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

InfraredWidget::InfraredWidget(QWidget *parent) : QDialog(parent)
{
    initUi();
    initCtrl();
    setModal(true);
}

InfraredWidget::~InfraredWidget()
{
    if (m_fd != -1) {
        ::close(m_fd);
    }

    QProcess::execute("rmmod /driver/sr501_drv.ko");
}

void InfraredWidget::initUi()
{
    setObjectName("infrared_widget");

    m_move.setFileName(":/misc/infraredwidget/images/run.gif");

    m_iconLbl.setObjectName("infrared_icon");
    m_iconLbl.setAlignment(Qt::AlignCenter);

    m_statusLbl.setObjectName("infrared_state");
    m_statusLbl.setAlignment(Qt::AlignCenter);

    QVBoxLayout *pLayout = new QVBoxLayout;

    pLayout->addSpacing(20);
    pLayout->addWidget(&m_iconLbl, 5);
    pLayout->addWidget(&m_statusLbl, 1);
    pLayout->setSpacing(100);
    setLayout(pLayout);
    CommonHelper::setStyleSheet(":/misc/infraredwidget/style/default.qss", this);
}

void InfraredWidget::initCtrl()
{
    QProcess::execute("insmod /driver/sr501_drv.ko");

    setStatus(false);
    connect(&m_timer, &QTimer::timeout, this, &InfraredWidget::timerTimeout);

    m_fd = ::open("/dev/sr501", O_RDONLY | O_NONBLOCK);
    if (m_fd != -1) {
        m_timer.start(20);
    }
    else {
        SimpleMessageBox::infomationMessageBox("未检测到设备，请重试");
    }
}

void InfraredWidget::setStatus(bool isActive)
{
    if (isActive) {
        m_statusLbl.setText("Have People");
        m_statusLbl.setStyleSheet("background: rgb(17, 209, 105)");
        m_iconLbl.setMovie(&m_move);
        m_move.start();
    }
    else {
        m_move.stop();
        m_statusLbl.setText("No One");
        m_statusLbl.setStyleSheet("background: rgb(221, 0, 27)");
        m_iconLbl.setPixmap(QPixmap());
    }
}

void InfraredWidget::timerTimeout()
{
    int status = 0;
    if (::read(m_fd, &status, 4) == 4) {
        setStatus(!!status);
    }
}
