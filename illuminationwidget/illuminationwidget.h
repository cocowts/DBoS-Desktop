#ifndef ILLUMINATIONWIDGET_H
#define ILLUMINATIONWIDGET_H

#include "wareprogressbar/wareprogressbar.h"
#include "illuminationthread.h"

#include <QDialog>
#include <QLabel>

class IlluminationWidget : public QDialog
{
    Q_OBJECT
public:
    explicit IlluminationWidget(QWidget *parent = nullptr);
    ~IlluminationWidget();

private:
    void initUi();
    void initCtrl();

private slots:
    void setValue(uint16_t ir, uint16_t ps, uint16_t als);

private:
    WareProgressBar m_irProgressBar;
    WareProgressBar m_psProgressBar;
    WareProgressBar m_alsProgressBar;

    QLabel m_irLbl;
    QLabel m_psLbl;
    QLabel m_alsLbl;

    IlluminationThread m_thread;
};

#endif // ILLUMINATIONWIDGET_H
