#ifndef OLEDWIDGET_H
#define OLEDWIDGET_H

#include "drawwidget.h"

#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include <QByteArray>
#include <QList>
#include <QVector>
#include <QFile>

class OledWidget : public QDialog
{
    Q_OBJECT
public:
    explicit OledWidget(QWidget *parent = nullptr);
    ~OledWidget();

private:
    void initUi();
    void initCtrl();
    void initRecord();

    QByteArray calFeature(QList<QList<QPoint>>* list);
    bool checkRecordIsValid(const QByteArray& input_record, QByteArray& output_record);
    int identifyRecord();

private slots:
    void studyBtnClicked();
    void clearBtnClicked();
    void identifyBtnClicked();

private:
    DrawWidget m_drawWidget;

    QComboBox m_numBox;
    QPushButton m_studyBtn;
    QPushButton m_clearBtn;
    QPushButton m_identifyBtn;

    QVector<QList<QByteArray>> m_record;
    QFile m_file;

    int m_fd = -1;
};

#endif // OLEDWIDGET_H
