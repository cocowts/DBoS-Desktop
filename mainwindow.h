#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QIcon>
#include <QPointer>
#include <QPushButton>
#include <QScopedPointer>
#include <QSize>
#include <QWidget>

#include "other/other.h"
#include "topwidget/topwidget.h"
#include "sliderwidget/sliderwidget.h"
#include "musicwidget/musicwidget.h"
#include "camerawidget/camerawidget.h"
#include "calculatorwidget/calculatorwidget.h"
#include "weatherwidget/weatherwidget.h"
#include "systemwidget/systemwidget.h"
#include "recorderwidget/recorderwidget.h"
#include "backlightwidget/backlightwidget.h"
#include "videowidget/videowidget.h"
#include "oledwidget/oledwidget.h"
#include "remotecontrolwidget/remotectrlwidget.h"
#include "ultrasonicwavewidget/ultrasonicwavewidget.h"
#include "photosensitivewidget/photosensitivewidget.h"
#include "electricitywidget/electricitywidget.h"
#include "infraredwidget/infraredwidget.h"
#include "illuminationwidget/illuminationwidget.h"
#include "keywidget/keywidget.h"
#include "mapwidget/mapwidget.h"
#include "temperaturewidget/temperaturewidget.h"

class MainWindow : public QWidget
{
    Q_OBJECT

    static constexpr int m_screenHeigh = 600;
    static constexpr int m_screenWidth = 1024;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void updateSysInfo();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    void initUi();
    void initCtrl();

    void setBackground(const QPixmap &pixmap);
    QWidget *initPage1();
    QWidget *initPage2();
    QPushButton *createButton(const QString &text, const QString &objName, QWidget *parent = nullptr);

private slots:
    void cameraBtnClicked();
    void musicBtnClicked();
    void calculatorBtnClicked();
    void weatherBtnClicked();
    void systemBtnClicked();
    void recorderBtnClicked();
    void backlightBtnClicked();
    void videoBtnClicked();
    void oledBtnClicked();
    void remoteCtrlBtnClicked();
    void ultrasonicwaveBtnClicked();
    void photosensitiveBtnClicked();
    void electricityBtnClicked();
    void infraredBtnClicked();
    void illuminationBtnClicked();
    void keyBtnClicked();
    void mapBtnClicked();
    void temperatureBtnClicked();

private:
    Other        *m_pOther        = new Other(this);
    TopWidget    *m_pTopWidget    = new TopWidget(this) ;
    SliderWidget *m_pSliderWidget = new SliderWidget(this);

    MusicWidget  *m_pMusicWidget = new MusicWidget("/music", this);
    QDialog *m_pGenericWidget    = nullptr;
};
#endif // MAINWINDOW_H
