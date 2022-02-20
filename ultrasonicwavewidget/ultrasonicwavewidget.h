#ifndef ULTRASONICWAVEWIDGET_H
#define ULTRASONICWAVEWIDGET_H

#include "ultrasonicwavethread.h"

#include <QDialog>
#include <QLabel>
#include <QThread>
#include <QSharedPointer>

class UltrasonicwaveWidget : public QDialog
{
    Q_OBJECT
public:
    explicit UltrasonicwaveWidget(QWidget *parent = nullptr);
    ~UltrasonicwaveWidget();

private:
    void initUi();
    void initCtrl();

private slots:
    void setDistance(int meter);

private:
    QLabel m_iconLbl;
    QLabel m_statusLbl;

    UltrasonicwaveThread m_thread;
};

#endif // ULTRASONICWAVEWIDGET_H
