#ifndef TEMPERATUREWIDGET_H
#define TEMPERATUREWIDGET_H

#include "dynamicline/dynamicline.h"
#include "temperaturethread.h"

#include <QDialog>
#include <QLabel>

class TemperatureWidget : public QDialog
{
    Q_OBJECT
public:
    explicit TemperatureWidget(QWidget *parent = nullptr);
    ~TemperatureWidget();

signals:

private:
    void initUi();
    void initCtrl();

private slots:
    void setValue(double temp, double humi);

private:
    DynamicLine m_tempLine;
    DynamicLine m_humiLine;

    QLabel m_tempLbl;
    QLabel m_humiLbl;

    TemperatureThread m_thread;
};

#endif // TEMPERATUREWIDGET_H
