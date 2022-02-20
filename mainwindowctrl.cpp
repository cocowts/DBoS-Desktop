#include "mainwindow.h"

#include <QDateTime>
#include <QLabel>
#include <QDateTime>
#include <QKeyEvent>

void MainWindow::initCtrl()
{
    updateSysInfo();

    m_pMusicWidget->hide();
    m_pMusicWidget->setFixedSize(this->size());
    m_pMusicWidget->setCursor(QCursor(QPixmap(":/misc/resource/image/point.png"), -1, -1));
}

void MainWindow::updateSysInfo()
{
    auto timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [&](){
        m_pTopWidget->setSysTimeText(QDateTime::currentDateTimeUtc().addSecs(60 * 60 * 8).toString("yyyy-MM-dd hh:mm:ss"));
        m_pTopWidget->setCpuInfoText(QString("%1%  %2℃").arg((int)m_pOther->getSysCpuUsage(), 3, 10, QLatin1Char('0')).arg(m_pOther->getCpuTemperature(), 2, 'g', 2, QLatin1Char('0')));
    });
    timer->start(999);
}

void MainWindow::cameraBtnClicked()
{
    m_pGenericWidget = new CameraWidget(this);
    m_pGenericWidget->installEventFilter(this);
    m_pGenericWidget->setFixedSize(this->size());
    m_pGenericWidget->setCursor(QCursor(QPixmap(":/misc/resource/image/point.png"), -1, -1));
    m_pGenericWidget->show();
}

void MainWindow::musicBtnClicked()
{
    m_pMusicWidget->show();
}

void MainWindow::calculatorBtnClicked()
{
    m_pGenericWidget = new CalculatorWidget(this);
    m_pGenericWidget->installEventFilter(this);
    m_pGenericWidget->setFixedSize(this->size());
    m_pGenericWidget->setCursor(QCursor(QPixmap(":/misc/resource/image/point.png"), -1, -1));
    m_pGenericWidget->show();
}

void MainWindow::weatherBtnClicked()
{
    m_pGenericWidget = new WeatherWidget(this);
    m_pGenericWidget->installEventFilter(this);
    m_pGenericWidget->setFixedSize(this->size());
    m_pGenericWidget->setCursor(QCursor(QPixmap(":/misc/resource/image/point.png"), -1, -1));
    m_pGenericWidget->show();
}

void MainWindow::systemBtnClicked()
{
    m_pGenericWidget = new SystemWidget(this);
    m_pGenericWidget->installEventFilter(this);
    m_pGenericWidget->setFixedSize(this->size());
    m_pGenericWidget->setCursor(QCursor(QPixmap(":/misc/resource/image/point.png"), -1, -1));
    m_pGenericWidget->show();
}

void MainWindow::recorderBtnClicked()
{
    m_pGenericWidget = new RecorderWidget(this);
    m_pGenericWidget->installEventFilter(this);
    m_pGenericWidget->setFixedSize(this->size());
    m_pGenericWidget->setCursor(QCursor(QPixmap(":/misc/resource/image/point.png"), -1, -1));
    m_pGenericWidget->show();
}

void MainWindow::backlightBtnClicked()
{
    m_pGenericWidget = new BacklightWidget(this);
    m_pGenericWidget->installEventFilter(this);
    m_pGenericWidget->setFixedSize(this->size());
    m_pGenericWidget->setCursor(QCursor(QPixmap(":/misc/resource/image/point.png"), -1, -1));
    m_pGenericWidget->show();
}

void MainWindow::videoBtnClicked()
{
    m_pGenericWidget = new VideoWidget("/video", this);
    m_pGenericWidget->installEventFilter(this);
    m_pGenericWidget->setFixedSize(this->size());
    m_pGenericWidget->setCursor(QCursor(QPixmap(":/misc/resource/image/point.png"), -1, -1));
    m_pGenericWidget->show();
}

void MainWindow::oledBtnClicked()
{
    m_pGenericWidget = new OledWidget(this);
    m_pGenericWidget->installEventFilter(this);
    m_pGenericWidget->setFixedSize(this->size());
    m_pGenericWidget->setCursor(QCursor(QPixmap(":/misc/resource/image/point.png"), -1, -1));
    m_pGenericWidget->show();
}

void MainWindow::remoteCtrlBtnClicked()
{
    m_pGenericWidget = new RemoteCtrlWidget(this);
    m_pGenericWidget->installEventFilter(this);
    m_pGenericWidget->setFixedSize(this->size());
    m_pGenericWidget->setCursor(QCursor(QPixmap(":/misc/resource/image/point.png"), -1, -1));
    m_pGenericWidget->show();
}

void MainWindow::ultrasonicwaveBtnClicked()
{
    m_pGenericWidget = new UltrasonicwaveWidget(this);
    m_pGenericWidget->installEventFilter(this);
    m_pGenericWidget->setFixedSize(this->size());
    m_pGenericWidget->setCursor(QCursor(QPixmap(":/misc/resource/image/point.png"), -1, -1));
    m_pGenericWidget->show();
}

void MainWindow::photosensitiveBtnClicked()
{
    m_pGenericWidget = new PhotosensitiveWidget(this);
    m_pGenericWidget->installEventFilter(this);
    m_pGenericWidget->setFixedSize(this->size());
    m_pGenericWidget->setCursor(QCursor(QPixmap(":/misc/resource/image/point.png"), -1, -1));
    m_pGenericWidget->show();
}

void MainWindow::electricityBtnClicked()
{
    m_pGenericWidget = new ElectricityWidget(this);
    m_pGenericWidget->installEventFilter(this);
    m_pGenericWidget->setFixedSize(this->size());
    m_pGenericWidget->setCursor(QCursor(QPixmap(":/misc/resource/image/point.png"), -1, -1));
    m_pGenericWidget->show();
}

void MainWindow::infraredBtnClicked()
{
    m_pGenericWidget = new InfraredWidget(this);
    m_pGenericWidget->installEventFilter(this);
    m_pGenericWidget->setFixedSize(this->size());
    m_pGenericWidget->setCursor(QCursor(QPixmap(":/misc/resource/image/point.png"), -1, -1));
    m_pGenericWidget->show();
}

void MainWindow::illuminationBtnClicked()
{
    m_pGenericWidget = new IlluminationWidget(this);
    m_pGenericWidget->installEventFilter(this);
    m_pGenericWidget->setFixedSize(this->size());
    m_pGenericWidget->setCursor(QCursor(QPixmap(":/misc/resource/image/point.png"), -1, -1));
    m_pGenericWidget->show();
}

void MainWindow::keyBtnClicked()
{
    m_pGenericWidget = new KeyWidget(this);
    m_pGenericWidget->installEventFilter(this);
    m_pGenericWidget->setFixedSize(this->size());
    m_pGenericWidget->setCursor(QCursor(QPixmap(":/misc/resource/image/point.png"), -1, -1));
    m_pGenericWidget->show();
}

void MainWindow::mapBtnClicked()
{
    m_pGenericWidget = new MapWidget(this);
    m_pGenericWidget->installEventFilter(this);
    m_pGenericWidget->setFixedSize(this->size());
    m_pGenericWidget->setCursor(QCursor(QPixmap(":/misc/resource/image/point.png"), -1, -1));
    m_pGenericWidget->show();
}

void MainWindow::temperatureBtnClicked()
{
    m_pGenericWidget = new TemperatureWidget(this);
    m_pGenericWidget->installEventFilter(this);
    m_pGenericWidget->setFixedSize(this->size());
    m_pGenericWidget->setCursor(QCursor(QPixmap(":/misc/resource/image/point.png"), -1, -1));
    m_pGenericWidget->show();
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    auto ptr = m_pGenericWidget;
    if ((ptr != nullptr) && (ptr == obj)) {
        if (event->type() == QEvent::KeyPress) {
            auto keyEvt = reinterpret_cast<QKeyEvent*>(event);
            if (keyEvt != nullptr && keyEvt->key() == Qt::Key_Escape) {
                removeEventFilter(ptr);
                delete ptr;
                return true;
            }
        }
        else if (event->type() == QEvent::Close) {
            removeEventFilter(ptr);
            delete ptr;
            return true;
        }
    }

    return false;
}


