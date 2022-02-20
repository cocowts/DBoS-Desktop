#include "backlightwidget.h"

#include "simplemessagebox/simplemessagebox.h"
#include "commonhelper.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

BacklightWidget::BacklightWidget(QWidget *parent) : QDialog(parent), m_file("/sys/class/backlight/backlight/brightness")
{
    initUi();
    initCtrl();
    setModal(true);
}

void BacklightWidget::initUi()
{
    m_iconLbl.setObjectName("backlight_icon");

    m_btn1.setAutoExclusive(true);
    m_btn1.setCheckable(true);
    m_btn2.setAutoExclusive(true);
    m_btn2.setCheckable(true);
    m_btn3.setAutoExclusive(true);
    m_btn3.setCheckable(true);
    m_btn4.setAutoExclusive(true);
    m_btn4.setCheckable(true);

    m_btn1.setChecked(true);

    m_btn1.setObjectName("backlight_btn1");
    m_btn2.setObjectName("backlight_btn2");
    m_btn3.setObjectName("backlight_btn3");
    m_btn4.setObjectName("backlight_btn4");

    auto pHLayout = new QHBoxLayout;
    pHLayout->addWidget(&m_btn1);
    pHLayout->addWidget(&m_btn2);
    pHLayout->addWidget(&m_btn3);
    pHLayout->addWidget(&m_btn4);

    auto pVLayout = new QVBoxLayout;
    pVLayout->addWidget(&m_iconLbl);
    pVLayout->addSpacing(100);
    pVLayout->addLayout(pHLayout);
    pVLayout->setContentsMargins(20, 20, 20, 100);

    setLayout(pVLayout);
    setObjectName("backlight_widget");
    CommonHelper::setStyleSheet(":/misc/backlightwidget/style/default.qss", this);
}

void BacklightWidget::initCtrl()
{
    int level = getBrightness1_4();

    if (level == -1) {
        SimpleMessageBox::infomationMessageBox("未找到设备节点，请检查");
    }
    else {
        if (level == 7)
            m_btn1.setChecked(true);
        else if (level == 8)
            m_btn2.setChecked(true);
        else if (level == 9)
            m_btn3.setChecked(true);
        else
            m_btn4.setChecked(true);
    }

    connect(&m_btn1, &QPushButton::clicked, this, &BacklightWidget::btnClicked);
    connect(&m_btn2, &QPushButton::clicked, this, &BacklightWidget::btnClicked);
    connect(&m_btn3, &QPushButton::clicked, this, &BacklightWidget::btnClicked);
    connect(&m_btn4, &QPushButton::clicked, this, &BacklightWidget::btnClicked);
}

void BacklightWidget::btnClicked()
{
    QPushButton *btn = qobject_cast<QPushButton *>(QObject::sender());
    if (btn == nullptr)
        return;

    if (btn == &m_btn1)      setBrightness1_4("7");
    else if (btn == &m_btn2) setBrightness1_4("8");
    else if (btn == &m_btn3) setBrightness1_4("9");
    else setBrightness1_4("10");
}

void BacklightWidget::setBrightness1_4(const char *level)
{
    if (m_file.open(QIODevice::WriteOnly)) {
        m_file.write(level);
        m_file.close();
    }
}

int BacklightWidget::getBrightness1_4()
{
    int brightness = -1;

    if (m_file.open(QIODevice::ReadOnly)) {
        brightness = m_file.read(4).toInt();
        if (brightness < 7)       brightness = 7;
        else if (brightness > 10) brightness = 10;
        m_file.close();
    }

    return brightness;
}

