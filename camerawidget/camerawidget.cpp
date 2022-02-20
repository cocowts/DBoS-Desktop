#include "camerawidget.h"

#include <QStringLiteral>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCameraInfo>
#include <QVariant>
#include <QCoreApplication>

#include "simplemessagebox/simplemessagebox.h"
#include "commonhelper.h"

CameraWidget::CameraWidget(QWidget *parent) : QDialog(parent, Qt::WindowStaysOnTopHint)
{
    initUi();
    initCtrl();
    setModal(true);
}

CameraWidget::~CameraWidget()
{
}

void CameraWidget::initUi()
{
    m_camComBox.setToolTip(QStringLiteral("可用的相机列表"));
    m_camComBox.setFocusPolicy(Qt::NoFocus);

    m_camResBox.setToolTip(QStringLiteral("可用的画面分辨率"));
    m_camResBox.setFocusPolicy(Qt::NoFocus);
    m_camResBox.setHidden(true);

    m_switchBtn.setText(QStringLiteral("打  开"));

    m_takePhotoBtn.setText(QStringLiteral("拍  照"));
    m_takePhotoBtn.setHidden(true);

    m_takeVideoBtn.setText(QStringLiteral("开始录像"));
    m_takeVideoBtn.setHidden(true);

    m_camScanBtn.setText(QStringLiteral("硬件扫描"));

    m_stateLbl.setObjectName(QStringLiteral("camStateLbl"));

    auto *pVBoxLayout = new QVBoxLayout();
    pVBoxLayout->addWidget(&m_camComBox);
    pVBoxLayout->addWidget(&m_camResBox);
    pVBoxLayout->addWidget(&m_switchBtn);
    pVBoxLayout->addWidget(&m_takePhotoBtn);
    pVBoxLayout->addWidget(&m_takeVideoBtn);
    pVBoxLayout->addWidget(&m_camScanBtn);
    pVBoxLayout->addWidget(&m_stateLbl);
    pVBoxLayout->setSpacing(35);
    pVBoxLayout->addStretch();
    pVBoxLayout->setContentsMargins(15, 15, 15, 15);
    m_ctrlWidget.setLayout(pVBoxLayout);
    m_ctrlWidget.setObjectName("ctrlWidget");

    auto *pHBoxLayout = new QHBoxLayout();
    pHBoxLayout->addWidget(&m_cameraViewfinder, 6);
    pHBoxLayout->addWidget(&m_ctrlWidget, 2);
    pHBoxLayout->setSpacing(0);
    pHBoxLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pHBoxLayout);
    setObjectName("dialog");
    CommonHelper::setStyleSheet(":/misc/camerawidget/style/default.qss", this);
}

void CameraWidget::initCtrl()
{
    connect(&m_camScanBtn, &QPushButton::clicked, this, &CameraWidget::updateCamInfo);
    connect(&m_switchBtn, &QPushButton::clicked, this, &CameraWidget::switchCamBtnClicked);
    connect(&m_takePhotoBtn, &QPushButton::clicked, this, &CameraWidget::takePhotoBtnClicked);
    connect(&m_takeVideoBtn, &QPushButton::clicked, this, &CameraWidget::takeVedioBtnClicked);
    connect(&m_camResBox, (void(QComboBox::*)(int))&QComboBox::currentIndexChanged, this, &CameraWidget::camResBoxChanged);

    updateCamInfo();

    m_cameraViewfinder.installEventFilter(this);
}

void CameraWidget::updateCamInfo()
{
    m_camComBox.clear();

    auto cameras = QCameraInfo::availableCameras();

    for (auto &camera : cameras) {
        m_camComBox.addItem(camera.description(), camera.deviceName());
    }

    if (m_camComBox.count() == 0)
        SimpleMessageBox::infomationMessageBox("未检测到相机,请检查后重试");
}

void CameraWidget::switchCamBtnClicked()
{
    if (m_camComBox.count() == 0) {
        SimpleMessageBox::infomationMessageBox("请插入相机后重试");
        return;
    }

    if (m_switchBtn.text() == "打  开") {
        m_camComBox.setEnabled(false);
        m_camResBox.setHidden(false);
        m_takePhotoBtn.setHidden(false);
        m_takeVideoBtn.setHidden(false);
        m_camScanBtn.setHidden(true);
        m_switchBtn.setText("关  闭");

        QCoreApplication::processEvents(QEventLoop::AllEvents, 10);

        m_camera.reset(new QCamera(m_camComBox.currentData(Qt::UserRole).toByteArray()));
        m_imageCapture.reset(new QCameraImageCapture(m_camera.data()));
        m_camera->setViewfinder(&m_cameraViewfinder);
        m_camera->start();
        connect(m_camera.data(), (void(QCamera::*)(QCamera::Error))&QCamera::error, this, &CameraWidget::displayCameraError, Qt::UniqueConnection);
        auto supportedResolutions = m_imageCapture->supportedResolutions();
        for (int i=supportedResolutions.count()-1; i>=0; --i) {
            const auto &resolution =  supportedResolutions.at(i);
            m_camResBox.addItem(QString("%1x%2").arg(resolution.width()).arg(resolution.height()), QVariant(resolution));
        }
    }
    else {
        m_camera->stop();
        m_switchBtn.setText("打  开");
        m_camComBox.setEnabled(true);
        m_camResBox.setHidden(true);
        m_takePhotoBtn.setHidden(true);
        m_takeVideoBtn.setHidden(true);
        m_camScanBtn.setHidden(false);
        m_camResBox.setHidden(true);
        m_camResBox.clear();
    }
}

void CameraWidget::takePhotoBtnClicked()
{
    m_imageCapture->capture();

    m_stateLbl.setEnabled(false);

    m_timer.singleShot(100, this, [&](){
        m_stateLbl.setEnabled(true);
    });
}

void CameraWidget::displayCameraError()
{
    SimpleMessageBox::errorMessageBox("发生严重错误，请重试");

    switchCamBtnClicked();
}

void CameraWidget::takeVedioBtnClicked()
{
    SimpleMessageBox::infomationMessageBox("IMX6ULL暂不支持此功能");
}

void CameraWidget::camResBoxChanged(int index)
{
    QImageEncoderSettings settings = m_imageCapture->encodingSettings();

    settings.setResolution(m_camResBox.itemData(index).toSize());

    m_imageCapture->setEncodingSettings(settings);
}

bool CameraWidget::eventFilter(QObject *o, QEvent *e)
{
    if (o == &m_cameraViewfinder && e->type() == QEvent::MouseButtonPress)
    {
        m_ctrlWidget.setHidden(!m_ctrlWidget.isHidden());
    }

    return false;
}
