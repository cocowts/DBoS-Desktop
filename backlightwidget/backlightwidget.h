#ifndef BACKLIGHTWIDGET_H
#define BACKLIGHTWIDGET_H

#include <QDialog>
#include <QLabel>
#include <QRadioButton>
#include <QPushButton>
#include <QFile>

class BacklightWidget : public QDialog
{
    Q_OBJECT
public:
    explicit BacklightWidget(QWidget *parent = nullptr);

private:
    void initUi();
    void initCtrl();

private slots:
    void btnClicked();
    void setBrightness1_4(const char *level = "7");
    int getBrightness1_4();

private:
    QLabel m_iconLbl;
    QPushButton m_btn1;
    QPushButton m_btn2;
    QPushButton m_btn3;
    QPushButton m_btn4;

    QFile m_file;
};

#endif // BACKLIGHTWIDGET_H
