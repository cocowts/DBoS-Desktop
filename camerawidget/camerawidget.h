#ifndef CAMERAWIDGET_H
#define CAMERAWIDGET_H

#include <QCamera>
#include <QCameraImageCapture>
#include <QCameraViewfinder>
#include <QComboBox>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QScopedPointer>
#include <QTimer>

class CameraWidget : public QDialog
{
    Q_OBJECT

public:
    explicit CameraWidget(QWidget *parent = nullptr);
    ~CameraWidget();

protected:
    bool eventFilter(QObject *o, QEvent *e);

protected slots:
    void updateCamInfo();
    void switchCamBtnClicked();
    void takePhotoBtnClicked();
    void takeVedioBtnClicked();
    void camResBoxChanged(int index);
    void displayCameraError();

private:
    void initUi();
    void initCtrl();

private:
    QWidget m_ctrlWidget;
    QComboBox m_camComBox;
    QComboBox m_camResBox;
    QPushButton m_switchBtn;
    QPushButton m_takePhotoBtn;
    QPushButton m_takeVideoBtn;
    QPushButton m_camScanBtn;
    QLabel m_stateLbl;
    QCameraViewfinder m_cameraViewfinder;

    QScopedPointer<QCameraImageCapture> m_imageCapture;
    QScopedPointer<QCamera> m_camera;
    QTimer m_timer;
};

#endif // CAMERAWIDGET_H
