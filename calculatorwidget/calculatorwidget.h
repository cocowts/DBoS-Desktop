#ifndef CALCULATORWIDGET_H
#define CALCULATORWIDGET_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QQueue>
#include <QChar>

class CalculatorWidget : public QDialog
{
    Q_OBJECT
public:
    explicit CalculatorWidget(QWidget *parent = nullptr);

private:
    void initUi();
    void initCtrl();

    bool isDigitOrDot(QChar c);
    bool isSymbol(QChar c);
    bool isSign(QChar c);
    bool isNumber(QString s);
    bool isOperator(QString s);
    bool isLeft(QString s);
    bool isRight(QString s);
    int priority(QString s);

    QQueue<QString> split(const QString& exp);
    bool match(const QQueue<QString>& exp);
    bool transform(QQueue<QString>& exp, QQueue<QString>& output);
    QString calculator(QQueue<QString>& exp);
    QString calculator(QString l, QString op, QString r);
    QString expression(const QString& exp);

private slots:
    void buttonClicked();

private:
    QLineEdit   m_edit;
    QPushButton m_buttons[20];
};

#endif // CALCULATORWIDGET_H
