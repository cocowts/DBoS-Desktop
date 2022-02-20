#include "remotectrlwidget.h"

#include "commonhelper.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QKeyEvent>

RemoteCtrlWidget::RemoteCtrlWidget(QWidget *parent) : QDialog(parent)
{
    initUi();
    initCtrl();
    setModal(true);
}

RemoteCtrlWidget::~RemoteCtrlWidget()
{ }

void RemoteCtrlWidget::initUi()
{
    m_powerBtn.setObjectName("remotecontrol_powerBtn");
    m_menuBtn.setObjectName("remotecontrol_menuBtn");

    m_testBtn.setObjectName("remotecontrol_testBtn");
    m_addBtn.setObjectName("remotecontrol_addBtn");
    m_recallBtn.setObjectName("remotecontrol_recallBtn");

    m_preBtn.setObjectName("remotecontrol_preBtn");
    m_playBtn.setObjectName("remotecontrol_playBtn");
    m_nextBtn.setObjectName("remotecontrol_nextBtn");

    m_num0Btn.setObjectName("remotecontrol_num0Btn");
    m_subBtn.setObjectName("remotecontrol_subBtn");
    m_numcBtn.setObjectName("remotecontrol_numcBtn");

    m_num1Btn.setObjectName("remotecontrol_num1Btn");
    m_num2Btn.setObjectName("remotecontrol_num2Btn");
    m_num3Btn.setObjectName("remotecontrol_num3Btn");

    m_num4Btn.setObjectName("remotecontrol_num4Btn");
    m_num5Btn.setObjectName("remotecontrol_num5Btn");
    m_num6Btn.setObjectName("remotecontrol_num6Btn");

    m_num7Btn.setObjectName("remotecontrol_num7Btn");
    m_num8Btn.setObjectName("remotecontrol_num8Btn");
    m_num9Btn.setObjectName("remotecontrol_num9Btn");

    auto pLayout = new QGridLayout;
    pLayout->addWidget(&m_powerBtn, 0, 0, Qt::AlignCenter);
    pLayout->addWidget(&m_menuBtn, 0, 2, Qt::AlignCenter);

    pLayout->addWidget(&m_testBtn, 1, 0, Qt::AlignCenter);
    pLayout->addWidget(&m_addBtn, 1, 1, Qt::AlignCenter);
    pLayout->addWidget(&m_recallBtn, 1, 2, Qt::AlignCenter);

    pLayout->addWidget(&m_preBtn, 2, 0, Qt::AlignCenter);
    pLayout->addWidget(&m_playBtn, 2, 1, Qt::AlignCenter);
    pLayout->addWidget(&m_nextBtn, 2, 2, Qt::AlignCenter);

    pLayout->addWidget(&m_num0Btn, 3, 0, Qt::AlignCenter);
    pLayout->addWidget(&m_subBtn, 3, 1, Qt::AlignCenter);
    pLayout->addWidget(&m_numcBtn, 3, 2, Qt::AlignCenter);

    pLayout->addWidget(&m_num1Btn, 4, 0, Qt::AlignCenter);
    pLayout->addWidget(&m_num2Btn, 4, 1, Qt::AlignCenter);
    pLayout->addWidget(&m_num3Btn, 4, 2, Qt::AlignCenter);

    pLayout->addWidget(&m_num4Btn, 5, 0, Qt::AlignCenter);
    pLayout->addWidget(&m_num5Btn, 5, 1, Qt::AlignCenter);
    pLayout->addWidget(&m_num6Btn, 5, 2, Qt::AlignCenter);

    pLayout->addWidget(&m_num7Btn, 6, 0, Qt::AlignCenter);
    pLayout->addWidget(&m_num8Btn, 6, 1, Qt::AlignCenter);
    pLayout->addWidget(&m_num9Btn, 6, 2, Qt::AlignCenter);
    pLayout->setSpacing(25);
    pLayout->setContentsMargins(20, 50, 20, 90);

    auto ctrlPanel = new QWidget(this);
    ctrlPanel->setObjectName("remotecontrol_ctrlPanel");
    ctrlPanel->setLayout(pLayout);

    auto pMainLayout = new QHBoxLayout;
    pMainLayout->addStretch();
    pMainLayout->addWidget(ctrlPanel);
    pMainLayout->addStretch();

    setLayout(pMainLayout);
    setObjectName("remotecontrol_widget");
    CommonHelper::setStyleSheet(":/misc/remotecontrolwidget/style/default.qss", this);
}

void RemoteCtrlWidget::initCtrl()
{
    m_map.insert(Qt::Key_NumLock, &m_powerBtn);
    m_map.insert(Qt::Key_Home, &m_menuBtn);
    m_map.insert(Qt::Key_F10, &m_testBtn);
    m_map.insert(Qt::Key_F6, &m_addBtn);
    m_map.insert(Qt::Key_F9, &m_recallBtn);
    m_map.insert(Qt::Key_6, &m_preBtn);
    m_map.insert(Qt::Key_Y, &m_playBtn);
    m_map.insert(Qt::Key_8, &m_nextBtn);
    m_map.insert(Qt::Key_U, &m_num0Btn);
    m_map.insert(Qt::Key_P, &m_subBtn);
    m_map.insert(Qt::Key_Equal, &m_numcBtn);
    m_map.insert(Qt::Key_4, &m_num1Btn);
    m_map.insert(Qt::Key_O, &m_num2Btn);
    m_map.insert(Qt::Key_1, &m_num3Btn);
    m_map.insert(Qt::Key_7, &m_num4Btn);
    m_map.insert(Qt::Key_3, &m_num5Btn);
    m_map.insert(Qt::Key_2, &m_num6Btn);
    m_map.insert(Qt::Key_F8, &m_num7Btn);
    m_map.insert(Qt::Key_Insert, &m_num8Btn);
    m_map.insert(Qt::Key_Minus, &m_num9Btn);

    m_timer.setSingleShot(true);
    connect(&m_timer, &QTimer::timeout, this, &RemoteCtrlWidget::timerTimeOut);
}

void RemoteCtrlWidget::setValue(int value)
{
    timerTimeOut();

    auto iterator = m_map.find(value);
    if (iterator == m_map.end())
        return;

    m_pCurBtn = iterator.value();
    m_pCurBtn->setDown(true);

    m_timer.start(200);
}

void RemoteCtrlWidget::timerTimeOut()
{
    if (m_pCurBtn != nullptr) {
        m_pCurBtn->setDown(false);
        m_pCurBtn = nullptr;
    }
}

void RemoteCtrlWidget::keyPressEvent(QKeyEvent *event)
{
    setValue(event->key());
}
