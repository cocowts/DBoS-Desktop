#include "videowidget.h"

#include "commonhelper.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QEvent>
#include <QKeyEvent>
#include <QCursor>
#include <QDir>
#include <QScrollBar>
#include <QScroller>
#include <QTime>

VideoWidget::VideoWidget(const QString &path, QWidget *parent) : QDialog(parent)
{
    initUi();
    initCtrl();
    loadVideo(path);
    setModal(true);
}

void VideoWidget::initUi()
{
    m_videoWidget.setObjectName("video_videoWidget");

    initCtrlWidget();
    initListWidget();

    auto pHLayout = new QHBoxLayout;
    pHLayout->addStretch();
    pHLayout->addWidget(&m_listWidget);
    pHLayout->setSpacing(0);
    pHLayout->setMargin(0);

    auto pVLayout = new QVBoxLayout;
    pVLayout->addStretch();
    pVLayout->addLayout(pHLayout, 10);
    pVLayout->addWidget(&m_ctrlWidget, 1);
    pVLayout->setMargin(0);
    pVLayout->setSpacing(0);
    m_videoWidget.setLayout(pVLayout);

    auto *pMainLayout = new QVBoxLayout;
    pMainLayout->addWidget(&m_videoWidget);
    pMainLayout->setMargin(0);
    pMainLayout->setSpacing(0);
    setLayout(pMainLayout);
    setObjectName("video_widget");
    CommonHelper::setStyleSheet(":/misc/videowidget/style/default.qss", this);
}

void VideoWidget::initCtrl()
{
    connect(&m_listWidget, &QListWidget::itemClicked, this, &VideoWidget::itemClicked);
    connect(&m_player, &QMediaPlayer::durationChanged, this, &VideoWidget::durationChanged);
    connect(&m_player, &QMediaPlayer::positionChanged, this, &VideoWidget::positionChanged);
    connect(&m_progressBarSlider, &QSlider::sliderPressed, this, &VideoWidget::progressBarSliderPressed);
    connect(&m_progressBarSlider, &QSlider::sliderReleased, this, &VideoWidget::progressBarSliderReleased);
    connect(&m_volumeSlider, &QSlider::valueChanged, this, &VideoWidget::volumeBarSliderValueChanged);
    connect(&m_playlist, &QMediaPlaylist::currentIndexChanged, this, &VideoWidget::currentIndexChanged);
    connect(&m_player, &QMediaPlayer::mutedChanged, this, &VideoWidget::mutedChanged);
    connect(&m_player, &QMediaPlayer::stateChanged, this, &VideoWidget::musicStateChanged);
    connect(&m_stopBtn, &QPushButton::clicked, this, &VideoWidget::stopBtnClicked);
    connect(&m_preBtn, &QPushButton::clicked, this, &VideoWidget::preBtnClicked);
    connect(&m_pauseBtn, &QPushButton::clicked, this, &VideoWidget::pauseBtnClicked);
    connect(&m_nextBtn, &QPushButton::clicked, this, &VideoWidget::nextBtnClicked);
    connect(&m_modeBtn, &QPushButton::clicked, this, &VideoWidget::modeBtnClicked);
    connect(&m_volumeBtn, &QPushButton::clicked, this, &VideoWidget::volumeBtnClicked);

    m_videoWidget.installEventFilter(this);

    m_playlist.setPlaybackMode(QMediaPlaylist::Loop);
    m_player.setPlaylist(&m_playlist);
    m_player.setVideoOutput(&m_videoWidget);

    m_player.setVolume(60);
    m_volumeSlider.setValue(60);
}

void VideoWidget::initCtrlWidget()
{
    m_ctrlWidget.setObjectName("video_ctrlWidget");

    m_curTimeLbl.setObjectName("video_curTimeLbl");
    m_delimiterLbl.setObjectName("video_delimiterLbl");
    m_totaTimelbl.setObjectName("video_totaTimelbl");
    m_stopBtn.setObjectName("video_stopBtn");
    m_preBtn.setObjectName("video_preBtn");
    m_pauseBtn.setObjectName("video_pauseBtn");
    m_nextBtn.setObjectName("video_nextBtn");
    m_volumeBtn.setObjectName("video_volumeBtn");
    m_volumeSlider.setObjectName("video_volumeSlider");
    m_modeBtn.setObjectName("video_modeBtn");
    m_progressBarSlider.setObjectName("video_progressBarSlider");

    m_curTimeLbl.setText("00:00:00");
    m_delimiterLbl.setText("/");
    m_totaTimelbl.setText("00:00:00");
    m_volumeSlider.setRange(0, 100);
    m_volumeSlider.setOrientation(Qt::Horizontal);
    m_progressBarSlider.setSingleStep(1000);
    m_progressBarSlider.setOrientation(Qt::Horizontal);

    auto pHLayout = new QHBoxLayout;
    pHLayout->addSpacing(20);
    pHLayout->addWidget(&m_curTimeLbl);
    pHLayout->addWidget(&m_delimiterLbl);
    pHLayout->addWidget(&m_totaTimelbl);
    pHLayout->addStretch();
    pHLayout->addWidget(&m_stopBtn);
    pHLayout->addSpacing(30);
    pHLayout->addWidget(&m_preBtn);
    pHLayout->addSpacing(30);
    pHLayout->addWidget(&m_pauseBtn);
    pHLayout->addSpacing(30);
    pHLayout->addWidget(&m_nextBtn);
    pHLayout->addSpacing(40);
    pHLayout->addWidget(&m_volumeBtn);
    pHLayout->addSpacing(5);
    pHLayout->addWidget(&m_volumeSlider);
    pHLayout->addStretch();
    pHLayout->addWidget(&m_modeBtn);
    pHLayout->addSpacing(20);
    pHLayout->setContentsMargins(0, 0, 0, 15);

    auto pVLayout = new QVBoxLayout;
    pVLayout->addWidget(&m_progressBarSlider);
    pVLayout->addLayout(pHLayout);
    pVLayout->setMargin(0);
    m_ctrlWidget.setLayout(pVLayout);
}

void VideoWidget::initListWidget()
{
    m_listWidget.setObjectName("video_listWidget");
    m_listWidget.verticalScrollBar()->setObjectName("video_verticalScrollBar");
    m_listWidget.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_listWidget.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_listWidget.setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_listWidget.setFocusPolicy(Qt::NoFocus);
    QScroller::grabGesture(&m_listWidget, QScroller::LeftMouseButtonGesture);

    m_listWidget.setMaximumWidth(300);
}

int VideoWidget::loadVideo(const QString &path)
{
    QStringList nameFilters = {"*.mp4", "*.mov", "*.wmv", "*.flv", "*.avi", "*.mkv"};
    auto infoList = QDir(path).entryInfoList(nameFilters, QDir::Files, QDir::Name);

    for (const auto &info : infoList) {
        m_listWidget.addItem(info.fileName());
        m_playlist.addMedia(QUrl::fromLocalFile(info.filePath()));
    }

    return m_listWidget.count();
}

void VideoWidget::itemClicked(QListWidgetItem *item)
{
    Q_UNUSED(item);

    if (m_playlist.currentIndex() != m_listWidget.currentRow()) {
        m_playlist.setCurrentIndex(m_listWidget.currentRow());
        m_player.play();
    }
}

void VideoWidget::durationChanged(qint64 duration)
{
    m_progressBarSlider.setRange(0, duration);
    m_totaTimelbl.setText(QTime::fromMSecsSinceStartOfDay(duration).toString("hh:mm:ss"));

}

void VideoWidget::positionChanged(qint64 position)
{
    if (m_player.state() == QMediaPlayer::StoppedState)
        return;

    if (!m_progressBarIsPressed)
        m_progressBarSlider.setValue(position);

    m_curTimeLbl.setText(QTime::fromMSecsSinceStartOfDay(position).toString("hh:mm:ss"));
}

void VideoWidget::progressBarSliderReleased()
{
    m_progressBarIsPressed = false;

    m_player.setPosition(m_progressBarSlider.value());

    if (m_player.state() == QMediaPlayer::PausedState)
        m_player.play();
}

void VideoWidget::progressBarSliderPressed()
{
    m_progressBarIsPressed = true;
}

void VideoWidget::volumeBarSliderValueChanged(int value)
{
    m_player.setVolume(value);

    if (m_player.isMuted())
        m_player.setMuted(false);
}

void VideoWidget::currentIndexChanged(int position)
{
    m_listWidget.setCurrentRow(position);
}

void VideoWidget::musicStateChanged(QMediaPlayer::State state)
{
    if (state == QMediaPlayer::PlayingState) {
        m_progressBarSlider.setEnabled(true);
        m_pauseBtn.setStyleSheet("image: url(:/misc/videowidget/images/pause.png);");
    }
    else {
        m_pauseBtn.setStyleSheet("image: url(:/misc/videowidget/images/play.png);");
    }

    if (state == QMediaPlayer::StoppedState) {
        m_progressBarSlider.setValue(0);
        m_curTimeLbl.setText("00:00:00");
    }
}

void VideoWidget::mutedChanged(bool muted)
{
    if (muted) {
        m_volumeBtn.setStyleSheet("image: url(:/misc/videowidget/images/mute.png);");
    }
    else {
        m_volumeBtn.setStyleSheet("image: url(:/misc/videowidget/images/sound.png);");
    }
}

void VideoWidget::stopBtnClicked()
{
    m_player.stop();
}

void VideoWidget::preBtnClicked()
{
    m_playlist.previous();

    if (m_player.state() != QMediaPlayer::PlayingState)
        m_player.play();
}

void VideoWidget::pauseBtnClicked()
{
    if (m_player.state() == QMediaPlayer::PlayingState) {
        m_player.pause();
    }
    else {
        m_player.play();
    }
}

void VideoWidget::nextBtnClicked()
{
    m_playlist.next();

    if (m_player.state() != QMediaPlayer::PlayingState) {
        m_player.play();
    }
}

void VideoWidget::modeBtnClicked()
{
    auto index =  (static_cast<int>(m_playlist.playbackMode()) + 1 ) % 5;

    if (index == 0)
        index++;

    m_playlist.setPlaybackMode(static_cast<QMediaPlaylist::PlaybackMode>(index));

    if (m_playlist.playbackMode() == QMediaPlaylist::CurrentItemInLoop) {
        m_modeBtn.setStyleSheet("image: url(:/misc/videowidget/images/currentitemInloop.png);");
    } else if (m_playlist.playbackMode() == QMediaPlaylist::Sequential) {
        m_modeBtn.setStyleSheet("image: url(:/misc/videowidget/images/sequential.png);");
    } else if (m_playlist.playbackMode() == QMediaPlaylist::Loop) {
        m_modeBtn.setStyleSheet("image: url(:/misc/videowidget/images/loop.png);");
    }  else if (m_playlist.playbackMode() == QMediaPlaylist::Random) {
        m_modeBtn.setStyleSheet("image: url(:/misc/videowidget/images/random.png);");
    }
}

void VideoWidget::volumeBtnClicked()
{
    m_player.setMuted(!m_player.isMuted());
}

bool VideoWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == &m_videoWidget) {
        if (event->type() == QEvent::MouseButtonPress) {
            auto area = this->height() / 20;
            if (QCursor::pos().y() < area * 10) {
                 m_listWidget.setHidden(!m_listWidget.isHidden());
            }
            else if (m_ctrlWidget.isHidden() || (QCursor::pos().y() < area * 18)) {
                 m_ctrlWidget.setHidden(!m_ctrlWidget.isHidden());
            }
        }
    }

    return false;
}

