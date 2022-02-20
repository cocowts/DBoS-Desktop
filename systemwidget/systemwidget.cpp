#include "systemwidget.h"

#include "commonhelper.h"

#include <QVBoxLayout>
#include <QGridLayout>
#include <QPixmap>
#include <QStorageInfo>
#include <QFile>
#include <QGuiApplication>
#include <QScreen>

SystemWidget::SystemWidget(QWidget *parent) : QDialog(parent)
{
    initUi();
    initCtrl();
    setModal(true);
}

void SystemWidget::initUi()
{
    m_devoceModeIcon.setPixmap(QPixmap(":/misc/systemwidget/images/id.png"));
    m_devoceModeIcon.setObjectName("system_devoceModeIcon");
    m_devoceModeTitle.setText("Device mode");
    m_devoceModeTitle.setObjectName("system_devoceModeTitle");
    m_devoceModeText.setText(getDeviceMode());
    m_devoceModeText.setObjectName("system_devoceModeText");
    m_devoceModeText.setAlignment(Qt::AlignTop);

    m_cpuIcon.setPixmap(QPixmap(":/misc/systemwidget/images/cpu.png"));
    m_cpuIcon.setObjectName("system_cpuIcon");
    m_cpuTitle.setText("CPU");
    m_cpuTitle.setObjectName("system_cpuTitle");
    m_cpuText.setText(getCpu());
    m_cpuText.setObjectName("system_cpuText");
    m_cpuText.setAlignment(Qt::AlignTop);

    m_ramIcon.setPixmap(QPixmap(":/misc/systemwidget/images/ram.png"));
    m_ramIcon.setObjectName("system_ramIcon");
    m_ramTitle.setText("RAM");
    m_ramTitle.setObjectName("system_ramTitle");
    m_ramText.setText(getRam());
    m_ramText.setObjectName("system_ramText");
    m_ramText.setAlignment(Qt::AlignTop);

    m_osVersionIcon.setPixmap(QPixmap(":/misc/systemwidget/images/ui.png"));
    m_osVersionIcon.setObjectName("system_osVersionIcon");
    m_osVersionTitle.setText("OS version Stable");
    m_osVersionTitle.setObjectName("system_osVersionTitle");
    m_osVersionText.setText(getOsVersion());
    m_osVersionText.setObjectName("system_osVersionText");
    m_osVersionText.setAlignment(Qt::AlignTop);

    m_resolutionIcon.setPixmap(QPixmap(":/misc/systemwidget/images/resolution.png"));
    m_resolutionIcon.setObjectName("system_resolutionIcon");
    m_resolutionTitle.setText("Resolution");
    m_resolutionTitle.setObjectName("system_resolutionTitle");
    m_resolutionText.setText(getResolution());
    m_resolutionText.setObjectName("system_resolutionText");
    m_resolutionText.setAlignment(Qt::AlignTop);

    m_storageIcon.setPixmap(QPixmap(":/misc/systemwidget/images/storage.png"));
    m_storageIcon.setObjectName("system_storageIcon");
    m_storageTitle.setText("Storage");
    m_storageTitle.setObjectName("system_storageTitle");
    m_storageText.setText(getStorage());
    m_storageText.setObjectName("system_storageText");
    m_storageText.setAlignment(Qt::AlignTop);

    auto *pLayoutV1 = new QVBoxLayout;
    pLayoutV1->addWidget(&m_devoceModeIcon);
    pLayoutV1->addWidget(&m_devoceModeTitle);
    pLayoutV1->addWidget(&m_devoceModeText);
    pLayoutV1->setSpacing(0);

    auto *pLayoutV2 = new QVBoxLayout;
    pLayoutV2->addWidget(&m_cpuIcon);
    pLayoutV2->addWidget(&m_cpuTitle);
    pLayoutV2->addWidget(&m_cpuText);
    pLayoutV2->setSpacing(0);

    auto *pLayoutV3 = new QVBoxLayout;
    pLayoutV3->addWidget(&m_ramIcon);
    pLayoutV3->addWidget(&m_ramTitle);
    pLayoutV3->addWidget(&m_ramText);
    pLayoutV3->setSpacing(0);

    auto *pLayoutV4 = new QVBoxLayout;
    pLayoutV4->addWidget(&m_osVersionIcon);
    pLayoutV4->addWidget(&m_osVersionTitle);
    pLayoutV4->addWidget(&m_osVersionText);
    pLayoutV4->setSpacing(0);

    auto *pLayoutV5 = new QVBoxLayout;
    pLayoutV5->addWidget(&m_resolutionIcon);
    pLayoutV5->addWidget(&m_resolutionTitle);
    pLayoutV5->addWidget(&m_resolutionText);
    pLayoutV5->setSpacing(0);

    auto *pLayoutV6 = new QVBoxLayout;
    pLayoutV6->addWidget(&m_storageIcon);
    pLayoutV6->addWidget(&m_storageTitle);
    pLayoutV6->addWidget(&m_storageText);
    pLayoutV6->setSpacing(0);

    auto *pMainLayout = new QGridLayout;

    pMainLayout->addLayout(pLayoutV1, 0, 0);
    pMainLayout->addLayout(pLayoutV2, 1, 0);
    pMainLayout->addLayout(pLayoutV3, 2, 0);
    pMainLayout->addLayout(pLayoutV4, 0, 1);
    pMainLayout->addLayout(pLayoutV5, 1, 1);
    pMainLayout->addLayout(pLayoutV6, 2, 1);
    pMainLayout->setVerticalSpacing(10);
    pMainLayout->setHorizontalSpacing(100);

    pMainLayout->setMargin(35);

    setLayout(pMainLayout);
    setObjectName("system_widget");
    CommonHelper::setStyleSheet(":/misc/systemwidget/style/default.qss", this);
}


void SystemWidget::initCtrl()
{

}

QString SystemWidget::getDeviceMode()
{
    return "10ASK_IM6ULL_PRO_BOARD";
}

QString SystemWidget::getCpu()
{
    QString cpuInfo;
    QFile file("/proc/cpuinfo");

    if (file.open(QIODevice::ReadOnly)) {
        QString info = file.readAll();

        auto index = info.indexOf("Hardware") + 11;
        if (index != -1) {
            cpuInfo = info.mid(index, info.mid(index, 60).indexOf("\n"));
        }

        file.close();
    }

    return cpuInfo;
}

QString SystemWidget::getRam()
{
    QString ramInfo;
    QFile file("/proc/meminfo");

    if (file.open(QIODevice::ReadOnly)) {
        QString info = file.readAll();
        double memtotal = 0.0;
        double memfree = 0.0;

        auto index = info.indexOf("MemTotal") + 10;
        if (index != -1) {
            memtotal = info.mid(index, info.mid(index, 60).indexOf("\n") - 3).trimmed().toUInt() / 1024 / 1024.0;
        }

        index = info.indexOf("MemFree") + 10;
        if (index != -1) {
            memfree = info.mid(index, info.mid(index, 60).indexOf("\n") - 3).trimmed().toUInt() / 1024 / 1024.0;
        }

        ramInfo = QString("%1GB / %2GB").arg( memtotal, 0, 'f', 2).arg(memfree, 0, 'f', 2);
    }

    return ramInfo;
}

QString SystemWidget::getOsVersion()
{
    return "DBoS 1.0.2 ";
}

QString SystemWidget::getResolution()
{
    auto rect = QGuiApplication::screens().at(0)->size();

    return QString("%1 x %2").arg(rect.width()).arg(rect.height());
}

QString SystemWidget::getStorage()
{
    QStorageInfo ROM = QStorageInfo::root();

    return QString("%1GB / %2GB").arg(ROM.bytesTotal()/1024/1024/1024.0, 0, 'f', 2).arg(ROM.bytesFree()/1024/1024/1024.0, 0, 'f', 2);
}
