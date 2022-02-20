#ifndef MUSICWIDGET_H
#define MUSICWIDGET_H

#include <QDesktopServices>
#include <QDialog>
#include <QDir>
#include <QFileInfo>
#include <QFuture>
#include <QFutureWatcher>
#include <QIcon>
#include <QLabel>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QPixmap>
#include <QPushButton>
#include <QSlider>
#include <QTableWidget>
#include <QVector>

class MusicWidget : public QDialog
{
Q_OBJECT

public:
    struct Mp3Info {
        QString title;     // 歌名
        QString album;     // 专辑
        QString singer;    // 歌手
        QPixmap  cover;    // 封面
        QString filePath;  // 歌曲路径
    };

public:
    explicit MusicWidget(const QString &path = "", QWidget *parent = nullptr);
    ~MusicWidget();

signals:

private:
    void initUi();
    void initCtrl();

    int loadSong(const QString &dir);

    void setBackground();
    QLayout *initLayout1();
    QLayout *initLayout2();
    QWidget *songListWidget();

    void addSongToList(const QString &title, const QString &singer, const QString &album, const QString &time);
    void setSongIconAtList(int index, const QIcon &icon);
    void restoreSongIconAtList(int index);

private slots:
     void cellDoubleClicked(int row, int column);
     void durationChanged(qint64 duration);
     void positionChanged(qint64 position);
     void progressBarSliderReleased();
     void progressBarSliderPressed();
     void volumeBarSliderValueChanged(int value);
     void currentIndexChanged(int position);
     void musicStateChanged(QMediaPlayer::State state);
     void mutedChanged(bool muted);

     void preBtnClicked();
     void pauseBtnClicked();
     void nextBtnClicked();
     void modeBtnClicked();
     void volumeBtnClicked();

     void addMp3Info(int index);

private:
    QLabel m_coverLbl;
    QLabel m_nameLbl;
    QLabel m_infoLbl;

    QPushButton m_preBtn;
    QPushButton m_pauseBtn;
    QPushButton m_nextBtn;
    QPushButton m_modeBtn;
    QPushButton m_volumeBtn;
    QSlider m_volumeSlider;
    QLabel m_curTimeLbl;
    QSlider m_progressBarSlider;
    QLabel m_totaTimelbl;
    QTableWidget m_tableWidget;

    QMediaPlaylist m_playlist;
    QMediaPlayer m_player;

    QVector<Mp3Info> m_musicVector;
    int m_preMusicIndex = -1;
    bool m_progressBarIsPressed = false;

    QFutureWatcher<Mp3Info> m_watcher;
};

Q_DECLARE_METATYPE(MusicWidget::Mp3Info);

#endif // MUSICWIDGET_H
