#ifndef TOPWIDGET_H
#define TOPWIDGET_H

#include <QLabel>
#include <QWidget>

class TopWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TopWidget(QWidget *parent = nullptr);

    void setCpuInfoText(const QString &text);
    void setSysTimeText(const QString &text);

private:
    QLabel *m_pCpuInfoLbl = new QLabel("100% 35.62℃", this);
    QLabel *m_pSysTimeLbl = new QLabel("2022-01-01 08:00:00", this);
};

#endif // TOPWIDGET_H
