#ifndef INFRAREDWIDGET_H
#define INFRAREDWIDGET_H

#include <QDialog>
#include <QLabel>
#include <QMovie>
#include <QTimer>

class InfraredWidget : public QDialog
{
    Q_OBJECT
public:
    explicit InfraredWidget(QWidget *parent = nullptr);
    ~InfraredWidget();

private:
    void initUi();
    void initCtrl();

private slots:
    void setStatus(bool isActive = false);
    void timerTimeout();

private:
    QLabel m_iconLbl;
    QLabel m_statusLbl;
    QMovie m_move;

    QTimer m_timer;
    int m_fd = -1;
};

#endif // INFRAREDWIDGET_H
