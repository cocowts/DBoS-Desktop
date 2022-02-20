#include "simplemessagebox.h"

#include "commonhelper.h"

SimpleMessageBox::SimpleMessageBox()
{
    this->setWindowFlag(Qt::FramelessWindowHint);

    initUi();
    initControl();
}

void SimpleMessageBox::initUi()
{
    //================ 组件属性设置
    m_mainLbl.setScaledContents(true);
    m_mainLbl.setFixedSize(64, 64);

    m_textLbl.setWordWrap(true);
    m_textLbl.setAlignment(Qt::AlignCenter);

    m_okBtn.setFocus();
    m_okBtn.setFixedSize(80, 35);
    m_cancleBtn.setFixedSize(80, 35);
    m_okBtn.setText(QStringLiteral("确 定"));
    m_cancleBtn.setText(QStringLiteral("取 消"));
    m_okBtn.setCursor(Qt::PointingHandCursor);
    m_cancleBtn.setCursor(Qt::PointingHandCursor);

    //================ 组件布局
    QHBoxLayout *pLayout1 = new QHBoxLayout;
    pLayout1->addWidget(&m_mainLbl);
    pLayout1->addSpacing(10);
    pLayout1->addWidget(&m_textLbl);

    QHBoxLayout *pLayout2 = new QHBoxLayout;
    pLayout2->addStretch(true);
    pLayout2->addWidget(&m_okBtn);
    pLayout2->addWidget(&m_cancleBtn);
    pLayout2->addSpacing(20);

    QVBoxLayout *pMainLayout = new QVBoxLayout;
    pMainLayout->addLayout(pLayout1);
    pMainLayout->addLayout(pLayout2);
    pMainLayout->setContentsMargins(10, 10, 10, 10);

    //================
    setLayout(pMainLayout);
    setMinimumSize(300, 40);
    CommonHelper::setStyleSheet(":/misc/simplemessagebox/style/default.qss", this);
}

void SimpleMessageBox::initControl()
{
    connect(&m_okBtn, &QPushButton::clicked, [=](){
        done(QDialog::Accepted);
        close();
    });

    connect(&m_cancleBtn, &QPushButton::clicked, [=](){
        done(QDialog::Rejected);
        close();
    });
}

int SimpleMessageBox::infomationMessageBox(const QString &text)
{
    SimpleMessageBox box;

    box.m_mainLbl.setPixmap(QPixmap(":/misc/simplemessagebox/images/info.png"));
    box.m_cancleBtn.setVisible(false);
    box.m_textLbl.setText(text);

    return box.exec();
}

int SimpleMessageBox::questionMessageBox(const QString &text)
{
    SimpleMessageBox box;

    box.m_mainLbl.setPixmap(QPixmap(":/misc/simplemessagebox/images/question.png"));
    box.m_textLbl.setText(text);

    return box.exec();
}

int SimpleMessageBox::errorMessageBox(const QString &text)
{
    SimpleMessageBox box;

    box.m_mainLbl.setPixmap(QPixmap(":/misc/simplemessagebox/images/error.png"));
    box.m_cancleBtn.setVisible(false);
    box.m_textLbl.setText(text);

    return box.exec();
}

void SimpleMessageBox::mouseMoveEvent(QMouseEvent *event)
{
    if (mousePressed)
    {
        this->move(event->globalPos() - mousePoint);
        event->accept();
    }
}

void SimpleMessageBox::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        mousePressed = true;
        mousePoint = event->globalPos() - this->pos();
        event->accept();
    }
}

void SimpleMessageBox::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)

    mousePressed = false;
}

