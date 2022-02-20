#ifndef PHOTOSENSITIVEWIDGET_H
#define PHOTOSENSITIVEWIDGET_H

#include "arcprogressbar/arcprogressbar.h"

#include <QDialog>
#include <QTimer>

class PhotosensitiveWidget : public QDialog
{
    Q_OBJECT
public:
    explicit PhotosensitiveWidget(QWidget *parent = nullptr);
    ~PhotosensitiveWidget();

protected slots:
    void timerTimeout();

private:
    void initUi();
    void initCtrl();

private:
    ArcProgressBar m_arcProgressBar;
    QTimer m_timer;

    int m_fd = -1;
};

#endif // PHOTOSENSITIVEWIDGET_H
