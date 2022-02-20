#include "keywidget.h"

#include "commonhelper.h"

#include <QVBoxLayout>
#include <QMovie>

KeyWidget::KeyWidget(QWidget *parent) : QDialog(parent)
{
    initUi();
    initCtrl();
    setModal(true);
}

void KeyWidget::initUi()
{
    m_lbl.setObjectName("key_icon");
    m_lbl.setText("点 击 任 意 按 键 继 续");
    m_lbl.setAlignment(Qt::AlignCenter);

    auto pLayout = new QVBoxLayout;
    pLayout->addWidget(&m_lbl);
    pLayout->setSpacing(0);
    pLayout->setMargin(0);
    setLayout(pLayout);

    setObjectName("key_widget");
    CommonHelper::setStyleSheet(":/misc/keywidget/style/default.qss", this);
}

void KeyWidget::initCtrl()
{ }

void KeyWidget::keyPressEvent(QKeyEvent *event)
{
    Q_UNUSED(event)

    if (m_lbl.movie() == nullptr) {
        m_lbl.setMovie(new QMovie(":/misc/keywidget/images/icon.gif", QByteArray(), &m_lbl));
    }

    m_isPlay = !m_isPlay;

    if (m_isPlay)
        m_lbl.movie()->start();
    else
        m_lbl.movie()->stop();
}

