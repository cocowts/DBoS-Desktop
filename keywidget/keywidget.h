#ifndef KEYWIDGET_H
#define KEYWIDGET_H

#include <QDialog>
#include <QLabel>

class KeyWidget : public QDialog
{
    Q_OBJECT
public:
    explicit KeyWidget(QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    void initUi();
    void initCtrl();

private:
    QLabel m_lbl;

    bool m_isPlay = false;
};

#endif // KEYWIDGET_H
