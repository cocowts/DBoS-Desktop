#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QSlider>
#include <QVideoWidget>
#include <QMediaPlaylist>
#include <QMediaPlayer>

class VideoWidget : public QDialog
{
    Q_OBJECT
public:
    explicit VideoWidget(const QString &path, QWidget *parent = nullptr);

protected:
    bool eventFilter(QObject *obj, QEvent *event);

protected slots:
    void itemClicked(QListWidgetItem *item);

    void durationChanged(qint64 duration);
    void positionChanged(qint64 position);
    void progressBarSliderReleased();
    void progressBarSliderPressed();
    void volumeBarSliderValueChanged(int value);
    void currentIndexChanged(int position);
    void musicStateChanged(QMediaPlayer::State state);
    void mutedChanged(bool muted);

    void stopBtnClicked();
    void preBtnClicked();
    void pauseBtnClicked();
    void nextBtnClicked();
    void modeBtnClicked();
    void volumeBtnClicked();

private:
    void initUi();
    void initCtrl();

    void initCtrlWidget();
    void initListWidget();

    int loadVideo(const QString &path);

private:
    QVideoWidget m_videoWidget;

    QWidget m_ctrlWidget;
    QPushButton m_stopBtn;
    QPushButton m_preBtn;
    QPushButton m_pauseBtn;
    QPushButton m_nextBtn;
    QPushButton m_modeBtn;
    QPushButton m_volumeBtn;
    QSlider m_volumeSlider;
    QLabel m_curTimeLbl;
    QSlider m_progressBarSlider;
    QLabel m_totaTimelbl;
    QLabel m_delimiterLbl;

    QListWidget m_listWidget;

    QMediaPlaylist m_playlist;
    QMediaPlayer m_player;

    bool m_progressBarIsPressed = false;
};

#endif // VIDEOWIDGET_H
