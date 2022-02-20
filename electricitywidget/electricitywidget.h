#ifndef ELECTRICITYWIDGET_H
#define ELECTRICITYWIDGET_H

#include <QDialog>

#include "colordashboard/colordashboard.h"

#include <QDialog>
#include <QSlider>
#include <QLabel>

class ElectricityWidget : public QDialog
{
    Q_OBJECT
public:
    explicit ElectricityWidget(QWidget *parent = nullptr);
    ~ElectricityWidget();

private:
    void initUi();
    void initCtrl();

private slots:
    void valueChanged(int value);

private:
    ColorDashboard m_dashboard;
    QSlider m_slider;
    QLabel m_valueLbl;

    int m_fd = -1;
};

#endif // ELECTRICITYWIDGET_H
