#ifndef SYSTEMWIDGET_H
#define SYSTEMWIDGET_H

#include <QDialog>
#include <QLabel>

class SystemWidget : public QDialog
{
    Q_OBJECT
public:
    explicit SystemWidget(QWidget *parent = nullptr);

private:
    void initUi();
    void initCtrl();

    QString getDeviceMode();
    QString getCpu();
    QString getRam();
    QString getOsVersion();
    QString getResolution();
    QString getStorage();

private:
    QLabel m_devoceModeIcon;
    QLabel m_devoceModeTitle;
    QLabel m_devoceModeText;

    QLabel m_cpuIcon;
    QLabel m_cpuTitle;
    QLabel m_cpuText;

    QLabel m_ramIcon;
    QLabel m_ramTitle;
    QLabel m_ramText;

    QLabel m_osVersionIcon;
    QLabel m_osVersionTitle;
    QLabel m_osVersionText;

    QLabel m_resolutionIcon;
    QLabel m_resolutionTitle;
    QLabel m_resolutionText;

    QLabel m_storageIcon;
    QLabel m_storageTitle;
    QLabel m_storageText;
};

#endif // SYSTEMWIDGET_H
