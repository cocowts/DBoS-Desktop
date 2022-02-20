#include "musicwidget.h"

#include "commonhelper.h"

#include <QFile>
#include <QFileInfoList>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QPixmap>
#include <QScrollBar>
#include <QScroller>
#include <QTextCodec>
#include <QTime>
#include <QVBoxLayout>
#include <QtConcurrent/QtConcurrent>

MusicWidget::Mp3Info getMp3BaseInfo(const QFileInfo &info)
{
    MusicWidget::Mp3Info ret;

    QFile file(info.filePath());

    if (!file.open(QIODevice::ReadOnly))
        return ret;

    QByteArray array = file.read(10);
    if (!(array.at(0) == 'I' && array.at(1) == 'D' && array.at(2) == '3'))
        return ret;

    for (uint8_t i=0; i<16; ++i)
    {
        array = file.read(10);
        file.read(1);

        QByteArray frameID     = array.mid(0, 4);
        uint32_t   size        = (uint8_t)array.at(4) * 0x1000000+(uint8_t)array.at(5) * 0x10000+(uint8_t)array.at(6) * 0x100+(uint8_t)array.at(7) - 1;
        QByteArray contentfile = file.read(size);

        if (frameID == "TALB") {
            ret.album = QTextCodec::codecForName("UTF-16")->toUnicode(contentfile);
        }
        else if (frameID == "TIT2") {
            ret.title = QTextCodec::codecForName("UTF-16")->toUnicode(contentfile);
        }
        else if (frameID == "TPE1") {
            ret.singer = QTextCodec::codecForName("UTF-16")->toUnicode(contentfile);
        }
        else if (frameID == "APIC") {
            contentfile.remove(0, 13);
            ret.cover.loadFromData(contentfile);
            break;
        }
    }

    ret.filePath = info.filePath();

    return ret;
}

MusicWidget::MusicWidget(const QString &path, QWidget *parent) : QDialog(parent)
{
    initUi();
    initCtrl();
    loadSong(path);
    setModal(true);
}

MusicWidget::~MusicWidget()
{
    QScroller::ungrabGesture(&m_tableWidget);
    m_watcher.cancel();
    m_watcher.waitForFinished();
}

void MusicWidget::initUi()
{
    auto *pMainLayout = new QVBoxLayout;
    pMainLayout->addLayout(initLayout1(), 0);
    pMainLayout->addSpacing(20);
    pMainLayout->addLayout(initLayout2(), 0);
    pMainLayout->addSpacing(20);
    pMainLayout->addWidget(songListWidget(), 1);
    pMainLayout->setSpacing(0);
    pMainLayout->setContentsMargins(20, 20, 20, 0);
    setLayout(pMainLayout);
    CommonHelper::setStyleSheet(":/misc/musicwidget/style/default.qss", this);
}

QLayout *MusicWidget::initLayout1()
{
    m_coverLbl.setPixmap(QPixmap(":/misc/musicwidget/images/cover.jpg").scaled(96, 96, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    m_coverLbl.setFixedSize(96, 96);
    m_coverLbl.setObjectName("coverLbl");

    m_nameLbl.setText("目及都是你");
    m_nameLbl.setAlignment(Qt::AlignBottom);
    m_nameLbl.setObjectName("nameLbl");

    m_infoLbl.setText("音乐笔记");
    m_infoLbl.setAlignment(Qt::AlignTop);
    m_infoLbl.setObjectName("infoLbl");

    auto *pLayout1 = new QVBoxLayout;
    pLayout1->addWidget(&m_nameLbl);
    pLayout1->addSpacing(1);
    pLayout1->addWidget(&m_infoLbl);
    pLayout1->setSpacing(0);
    pLayout1->setContentsMargins(0, 0, 0, 0);

    auto *pLayout2 = new QHBoxLayout();
    pLayout2->addWidget(&m_coverLbl);
    pLayout2->addSpacing(10);
    pLayout2->addLayout(pLayout1);
    pLayout2->addSpacing(0);
    pLayout2->setMargin(0);

    return pLayout2;
}

QLayout *MusicWidget::initLayout2()
{
    auto *pLayout = new QHBoxLayout;

    m_preBtn.setObjectName("preBtn");
    m_pauseBtn.setObjectName("pauseBtn");
    m_nextBtn.setObjectName("nextBtn");
    m_modeBtn.setObjectName("modeBtn");
    m_volumeBtn.setObjectName("volumeBtn");

    m_volumeSlider.setOrientation(Qt::Horizontal);
    m_volumeSlider.setObjectName("volumeSlider");
    m_volumeSlider.setRange(0, 100);

    m_curTimeLbl.setText("0:00");
    m_curTimeLbl.setObjectName("curTimeLbl");

    m_progressBarSlider.setOrientation(Qt::Horizontal);
    m_progressBarSlider.setObjectName("progressBarSlider");
    m_progressBarSlider.setSingleStep(1000);

    m_totaTimelbl.setText("0:00");
    m_totaTimelbl.setObjectName("totaTimeLbl");

    pLayout->addWidget(&m_preBtn);
    pLayout->addSpacing(30);
    pLayout->addWidget(&m_pauseBtn);
    pLayout->addSpacing(30);
    pLayout->addWidget(&m_nextBtn);
    pLayout->addSpacing(30);
    pLayout->addWidget(&m_modeBtn);
    pLayout->addSpacing(30);
    pLayout->addWidget(&m_volumeBtn);
    pLayout->addSpacing(5);
    pLayout->addWidget(&m_volumeSlider);
    pLayout->addSpacing(30);
    pLayout->addWidget(&m_curTimeLbl);
    pLayout->addSpacing(5);
    pLayout->addWidget(&m_progressBarSlider);
    pLayout->addSpacing(5);
    pLayout->addWidget(&m_totaTimelbl);

    pLayout->setSpacing(0);
    pLayout->setMargin(0);

    return pLayout;
}

QWidget *MusicWidget::songListWidget()
{
    m_tableWidget.setObjectName("listWidget");

    m_tableWidget.setRowCount(0);
    m_tableWidget.setColumnCount(5);

    QStringList headers;
    headers << "" << "标题" << "歌手" << "专辑" << "时间";
    m_tableWidget.setHorizontalHeaderLabels(headers);

    m_tableWidget.setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tableWidget.setSelectionMode(QTableView::SingleSelection);
    m_tableWidget.setSelectionBehavior(QTableView::SelectRows);
    m_tableWidget.horizontalHeader()->setHighlightSections(false);
    m_tableWidget.horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_tableWidget.verticalHeader()->hide();
    m_tableWidget.verticalHeader()->setDefaultSectionSize(45);
    m_tableWidget.setSortingEnabled(false);
    m_tableWidget.setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_tableWidget.setFocusPolicy(Qt::NoFocus);
    QScroller::grabGesture(&m_tableWidget, QScroller::LeftMouseButtonGesture);

    m_tableWidget.horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    m_tableWidget.setColumnWidth(0, 20);
    m_tableWidget.horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_tableWidget.horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    m_tableWidget.setColumnWidth(2, 280);
    m_tableWidget.horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
    m_tableWidget.setColumnWidth(3, 200);
    m_tableWidget.horizontalHeader()->setSectionResizeMode(4, QHeaderView::Fixed);
    m_tableWidget.setColumnWidth(4, 100);

    return &m_tableWidget;
}

int MusicWidget::loadSong(const QString &path)
{
    QStringList nameFilters = {"*.mp3"};
    auto infoList = QDir(path).entryInfoList(nameFilters, QDir::Files, QDir::Name);
    m_watcher.setFuture(QtConcurrent::mapped(infoList, getMp3BaseInfo));
    return infoList.count();
}

void MusicWidget::addSongToList(const QString &title, const QString &singer, const QString &album, const QString &time)
{
    auto cnt = m_tableWidget.rowCount();

    m_tableWidget.insertRow(cnt);

    m_tableWidget.setItem(cnt, 0, new QTableWidgetItem(QString("%1").arg(cnt, 2, 10, QLatin1Char('0'))));
    m_tableWidget.setItem(cnt, 1, new QTableWidgetItem(title));
    m_tableWidget.setItem(cnt, 2, new QTableWidgetItem(singer));
    m_tableWidget.setItem(cnt, 3, new QTableWidgetItem(album));
    m_tableWidget.setItem(cnt, 4, new QTableWidgetItem(time));
}

void MusicWidget::setSongIconAtList(int index, const QIcon &icon)
{
    if (index < 0 || index >= m_tableWidget.rowCount())
        return;

    auto tableWidgetItem = m_tableWidget.item(index, 0);

    tableWidgetItem->setIcon(icon);
    tableWidgetItem->setText("");
}

void MusicWidget::restoreSongIconAtList(int index)
{
    if (index < 0 || index >= m_tableWidget.rowCount())
        return;

    auto tableWidgetItem = m_tableWidget.item(index, 0);

    tableWidgetItem->setIcon(QIcon());
    tableWidgetItem->setText(QString("%1").arg(index, 2, 10, QLatin1Char('0')));
}

void MusicWidget::addMp3Info(int index)
{
    auto info = m_watcher.resultAt(index);

    if (info.filePath.isEmpty())
        return;

    addSongToList(info.title, info.singer, info.album, "00:00");
    m_musicVector.append(info);
    m_playlist.addMedia(QUrl::fromLocalFile(info.filePath));
}

void MusicWidget::cellDoubleClicked(int row, int column)
{
    Q_UNUSED(column);

    if (row != m_playlist.currentIndex()) {
        m_playlist.setCurrentIndex(row);
        m_player.play();
    }
}

void MusicWidget::durationChanged(qint64 duration)
{
    m_progressBarSlider.setRange(0, duration);
    m_totaTimelbl.setText(QTime::fromMSecsSinceStartOfDay(duration).toString("mm:ss"));

    auto tableWidgetItem = m_tableWidget.item(m_playlist.currentIndex(), 4);
    tableWidgetItem->setText(QTime::fromMSecsSinceStartOfDay(duration).toString("mm:ss"));
}

void MusicWidget::positionChanged(qint64 position)
{
    if (!m_progressBarIsPressed)
        m_progressBarSlider.setValue(position);

    m_curTimeLbl.setText(QTime::fromMSecsSinceStartOfDay(position).toString("mm:ss"));
}

void MusicWidget::progressBarSliderPressed()
{
    m_progressBarIsPressed = true;
}

void MusicWidget::progressBarSliderReleased()
{
    m_progressBarIsPressed = false;

    m_player.setPosition(m_progressBarSlider.value());

    if (m_player.state() == QMediaPlayer::PausedState)
        m_player.play();
}

void MusicWidget::volumeBarSliderValueChanged(int value)
{
    m_player.setVolume(value);

    if (m_player.isMuted())
        m_player.setMuted(false);
}

void MusicWidget::currentIndexChanged(int position)
{
    if (m_preMusicIndex != -1)
        restoreSongIconAtList(m_preMusicIndex);

    m_preMusicIndex = position;

    setSongIconAtList(position, QIcon(":/misc/musicwidget/images/playing.png"));

    m_nameLbl.setText(m_musicVector.at(position).title);
    m_infoLbl.setText(m_musicVector.at(position).singer + " - " + m_musicVector.at(position).album);
    m_coverLbl.setPixmap(m_musicVector.at(position).cover.scaled(96, 96, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
}

void MusicWidget::musicStateChanged(QMediaPlayer::State state)
{
    if (state == QMediaPlayer::PlayingState) {
        m_pauseBtn.setStyleSheet("image: url(:/misc/musicwidget/images/pause.png);");
    }
    else {
        m_pauseBtn.setStyleSheet("image: url(:/misc/musicwidget/images/play.png);");
    }
}

void MusicWidget::mutedChanged(bool muted)
{
    if (muted) {
        m_volumeBtn.setStyleSheet("image: url(:/misc/musicwidget/images/mute.png);");
    }
    else {
        m_volumeBtn.setStyleSheet("image: url(:/misc/musicwidget/images/sound.png);");
    }
}

void MusicWidget::preBtnClicked()
{
    m_playlist.previous();

    if (m_player.state() != QMediaPlayer::PlayingState)
        m_player.play();
}

void MusicWidget::pauseBtnClicked()
{
    if (m_player.state() == QMediaPlayer::PlayingState) {
        m_player.pause();
    }
    else {
        m_player.play();
    }
}

void MusicWidget::nextBtnClicked()
{
    m_playlist.next();

    if (m_player.state() != QMediaPlayer::PlayingState) {
        m_player.play();
    }
}

void MusicWidget::modeBtnClicked()
{
    auto index =  (static_cast<int>(m_playlist.playbackMode()) + 1 ) % 5;

    if (index == 0)
        index++;

    m_playlist.setPlaybackMode(static_cast<QMediaPlaylist::PlaybackMode>(index));

    if (m_playlist.playbackMode() == QMediaPlaylist::CurrentItemInLoop) {
        m_modeBtn.setStyleSheet("image: url(:/misc/musicwidget/images/currentitemInloop.png);");
    } else if (m_playlist.playbackMode() == QMediaPlaylist::Sequential) {
        m_modeBtn.setStyleSheet("image: url(:/misc/musicwidget/images/sequential.png);");
    } else if (m_playlist.playbackMode() == QMediaPlaylist::Loop) {
        m_modeBtn.setStyleSheet("image: url(:/misc/musicwidget/images/loop.png);");
    }  else if (m_playlist.playbackMode() == QMediaPlaylist::Random) {
        m_modeBtn.setStyleSheet("image: url(:/misc/musicwidget/images/random.png);");
    }
}

void MusicWidget::volumeBtnClicked()
{
    m_player.setMuted(!m_player.isMuted());
}

void MusicWidget::initCtrl()
{
    connect(&m_watcher, &QFutureWatcher<int>::resultReadyAt, this, &MusicWidget::addMp3Info);
    connect(&m_tableWidget, &QTableWidget::cellClicked, this, &MusicWidget::cellDoubleClicked);
    connect(&m_player, &QMediaPlayer::durationChanged, this, &MusicWidget::durationChanged);
    connect(&m_player, &QMediaPlayer::positionChanged, this, &MusicWidget::positionChanged);
    connect(&m_progressBarSlider, &QSlider::sliderPressed, this, &MusicWidget::progressBarSliderPressed);
    connect(&m_progressBarSlider, &QSlider::sliderReleased, this, &MusicWidget::progressBarSliderReleased);
    connect(&m_volumeSlider, &QSlider::valueChanged, this, &MusicWidget::volumeBarSliderValueChanged);
    connect(&m_playlist, &QMediaPlaylist::currentIndexChanged, this, &MusicWidget::currentIndexChanged);
    connect(&m_player, &QMediaPlayer::mutedChanged, this, &MusicWidget::mutedChanged);
    connect(&m_player, &QMediaPlayer::stateChanged, this, &MusicWidget::musicStateChanged);
    connect(&m_preBtn, &QPushButton::clicked, this, &MusicWidget::preBtnClicked);
    connect(&m_pauseBtn, &QPushButton::clicked, this, &MusicWidget::pauseBtnClicked);
    connect(&m_nextBtn, &QPushButton::clicked, this, &MusicWidget::nextBtnClicked);
    connect(&m_modeBtn, &QPushButton::clicked, this, &MusicWidget::modeBtnClicked);
    connect(&m_volumeBtn, &QPushButton::clicked, this, &MusicWidget::volumeBtnClicked);

    m_playlist.setPlaybackMode(QMediaPlaylist::Loop);
    m_player.setPlaylist(&m_playlist);

    m_player.setVolume(60);
    m_volumeSlider.setValue(60);
}
