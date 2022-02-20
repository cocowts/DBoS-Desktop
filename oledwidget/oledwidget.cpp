#include "oledwidget.h"

#include "simplemessagebox/simplemessagebox.h"
#include "commonhelper.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QtMath>
#include <QProcess>

OledWidget::OledWidget(QWidget *parent) : QDialog(parent)
{
    initUi();
    initCtrl();
    setModal(true);
}

OledWidget::~OledWidget()
{
    if (m_fd != -1) {
        ::close(m_fd);
    }

    QProcess::execute("rmmod /driver/oled_drv.ko");
}


void OledWidget::initUi()
{
    QStringList numList;
    numList << "0" << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9";
    m_numBox.addItems(numList);
    m_numBox.setObjectName("oled_numBox");
    m_numBox.setFocusPolicy(Qt::NoFocus);
    m_numBox.setStyleSheet("QComboBox#oled_numBox QAbstractItemView {background: rgb(75, 75, 75);border: none;font: normal normal 28px;outline: none;}");

    m_studyBtn.setText("学习");
    m_studyBtn.setObjectName("oled_studyBtn");

    m_clearBtn.setText("清空");
    m_clearBtn.setObjectName("oled_clearBtn");

    m_identifyBtn.setText("识别");
    m_identifyBtn.setObjectName("oled_identifyBtn");

    auto pVLayout = new QVBoxLayout;
    pVLayout->addWidget(&m_numBox);
    pVLayout->addWidget(&m_studyBtn);
    pVLayout->addWidget(&m_clearBtn);
    pVLayout->addWidget(&m_identifyBtn);
    pVLayout->addStretch();
    pVLayout->setMargin(0);
    pVLayout->setSpacing(35);
    pVLayout->setContentsMargins(15, 15, 15, 15);

    auto pCtrlWidget = new QWidget(this);
    pCtrlWidget->setObjectName("oled_ctrlwidget");
    pCtrlWidget->setLayout(pVLayout);

    auto pHLayout = new QHBoxLayout;
    pHLayout->addWidget(&m_drawWidget, 5);
    pHLayout->addWidget(pCtrlWidget, 1);
    pHLayout->setMargin(0);

    setLayout(pHLayout);
    setObjectName("oled_widget");
    CommonHelper::setStyleSheet(":/misc/oledwidget/style/default.qss", this);
}

void OledWidget::initCtrl()
{
    QProcess::execute("insmod /driver/oled_drv.ko");

    m_fd = ::open("/dev/myoled", O_WRONLY);
    if (m_fd < 0) {
        SimpleMessageBox::infomationMessageBox("未检测到设备，请重试");
    }

    initRecord();

    connect(&m_studyBtn, &QPushButton::clicked, this, &OledWidget::studyBtnClicked);
    connect(&m_clearBtn, &QPushButton::clicked, this, &OledWidget::clearBtnClicked);
    connect(&m_identifyBtn, &QPushButton::clicked, this, &OledWidget::identifyBtnClicked);
}

void OledWidget::initRecord()
{
    m_record.resize(10);

    for(int i=0; i<10; i++)
        m_record[i].clear();
}

QByteArray OledWidget::calFeature(QList<QList<QPoint>>* list)
{
    QByteArray array(10, 0);

    int x1 = 0xFFFFFF;
    int y1 = 0xFFFFFF;
    int x2 = 0;
    int y2 = 0;

    // 寻找矩形坐标
    for (int i=0; i<list->length(); i++) {
        for(int j=0; j<(*list)[i].length(); j++) {
            if (x1 > (*list)[i][j].x())
                x1 = (*list)[i][j].x();

            if (y1 > (*list)[i][j].y())
                y1 = (*list)[i][j].y();

            if (x2 < (*list)[i][j].x())
                x2 = (*list)[i][j].x();

            if (y2 < (*list)[i][j].y())
                y2 = (*list)[i][j].y();
        }
    }

    // 提取9向量点数量
    int w_divide = (x2 - x1) / 3;
    int h_divide = (y2 - y1) / 3;

    for (int i=0; i<list->length(); i++) {
        for(int j=0; j<(*list)[i].length(); j++) {
            if (((*list)[i][j].x() / (x1 + 2 * w_divide) != 0) && ((*list)[i][j].y() / (y1 + 2 * h_divide) != 0)) {
                array[8] = array[8]+1;
            }
            else if (((*list)[i][j].x() / (x1 + 1 * w_divide) != 0) && ((*list)[i][j].y() / (y1 + 2 * h_divide) != 0)) {
                array[7] = array[7]+1;
            }
            else if (((*list)[i][j].x() / (x1 + 0 * w_divide) != 0) && ((*list)[i][j].y() / (y1 + 2 * h_divide) != 0)) {
                array[6] = array[6]+1;
            }
            else if (((*list)[i][j].x() / (x1 + 2 * w_divide) != 0) && ((*list)[i][j].y() / (y1 + 1 * h_divide) != 0)) {
                array[5] = array[5]+1;
            }
            else if (((*list)[i][j].x() / (x1 + 1 * w_divide) != 0) && ((*list)[i][j].y() / (y1 + 1 * h_divide) != 0)) {
                array[4] = array[4]+1;
            }
            else if (((*list)[i][j].x() / (x1 + 0 * w_divide) != 0) && ((*list)[i][j].y() / (y1 + 1 * h_divide) != 0)) {
                array[3] = array[3]+1;
            }
            else if (((*list)[i][j].x() / (x1 + 2 * w_divide) != 0) && ((*list)[i][j].y() / (y1 + 0 * h_divide) != 0)) {
                array[2] = array[2]+1;
            }
            else if (((*list)[i][j].x() / (x1 + 1 * w_divide) != 0) && ((*list)[i][j].y() / (y1 + 0 * h_divide) != 0)) {
                array[1] = array[1]+1;
            }
            else if (((*list)[i][j].x() / (x1 + 0 * w_divide) != 0) && ((*list)[i][j].y() / (y1 + 0 * h_divide) != 0)) {
                array[0] = array[0]+1;
            }
        }
    }

    return array;
}

bool OledWidget::checkRecordIsValid(const QByteArray& input_record, QByteArray& output_record)
{
    bool ret = true;
    int i = 0;

    for(i=0; i<10; i++) {
        if (input_record.at(i) != 0) {
            break;
        }
    }

    if (i == 10) {
        SimpleMessageBox::infomationMessageBox("请首先写出数字喔~~~   ");
        ret = false;
    }

    output_record = input_record;

    return ret;
}

int OledWidget::identifyRecord()
{
    QVector<int> t(10, 0);
    double min = 999999.999999;
    int index = -1;

    // 特征提取
    QByteArray features;
    if (!checkRecordIsValid(calFeature(m_drawWidget.getDraw()), features)) {
        return index;
    }

    // 欧拉距离特征比较
    for (int i=0; i<10; i++) {
        for (int j=0; j<m_record[i].length(); j++) {
            for (int k=0; k<10; k++) {
                t[k] = m_record[i][j][k] - features[k];
                t[k] = t[k] * t[k];
            }

            double val = qSqrt(t[0] + t[1] + t[2]+ t[3]+ t[4]+ t[5]+ t[6]+ t[7]+ t[8]+ t[9]);

            if (min > val) {
                min = val;
                index = i;
            }
        }
    }

    return index;
}

void OledWidget::studyBtnClicked()
{
    QByteArray record;
    if (!checkRecordIsValid(calFeature(m_drawWidget.getDraw()), record)) {
        return;
    }

    auto index = m_numBox.currentIndex();
    if (SimpleMessageBox::questionMessageBox("是否要对 " + QString::number(index) + " 进行学习？") == SimpleMessageBox::Accepted) {
        m_record[index].append(record);
    }
}

void OledWidget::clearBtnClicked()
{
    m_drawWidget.clear();
}

void OledWidget::identifyBtnClicked()
{
    int index = identifyRecord();

    if (index == -1) {
        SimpleMessageBox::infomationMessageBox("请首先教我识字喔~~~");
    }
    else {
        if (m_fd > 0) {
            char data[3];
            data[0] = 63;
            data[1] = 4;
            data[2] = index + 48;
            ::write(m_fd, data, 3);
        }

        SimpleMessageBox::infomationMessageBox("本次结果 : [ " + QString::number(index) + " ]");
    }
}

