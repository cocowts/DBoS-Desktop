#ifndef REMOTECTRLWIDGET_H
#define REMOTECTRLWIDGET_H

#include <QDialog>
#include <QPushButton>
#include <QTimer>
#include <QMap>

class RemoteCtrlWidget : public QDialog
{
    Q_OBJECT
public:
    explicit RemoteCtrlWidget(QWidget *parent = nullptr);
    ~RemoteCtrlWidget();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    void initUi();
    void initCtrl();

private slots:
    void setValue(int value);
    void timerTimeOut();

private:
    QPushButton m_powerBtn;
    QPushButton m_menuBtn;
    QPushButton m_testBtn;
    QPushButton m_addBtn;
    QPushButton m_recallBtn;
    QPushButton m_preBtn;
    QPushButton m_playBtn;
    QPushButton m_nextBtn;
    QPushButton m_num0Btn;
    QPushButton m_subBtn;
    QPushButton m_numcBtn;
    QPushButton m_num1Btn;
    QPushButton m_num2Btn;
    QPushButton m_num3Btn;
    QPushButton m_num4Btn;
    QPushButton m_num5Btn;
    QPushButton m_num6Btn;
    QPushButton m_num7Btn;
    QPushButton m_num8Btn;
    QPushButton m_num9Btn;

    QTimer m_timer;
    QMap<int, QPushButton*> m_map;
    QPushButton *m_pCurBtn = nullptr;
};

#endif // REMOTECTRLWIDGsET_H
