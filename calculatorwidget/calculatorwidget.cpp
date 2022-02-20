#include "calculatorwidget.h"

#include "commonhelper.h"

#include <QGridLayout>
#include <QSizePolicy>
#include <QStack>
#include <QRegExp>

CalculatorWidget::CalculatorWidget(QWidget *parent) : QDialog(parent)
{
    initUi();
    initCtrl();
    setModal(true);
}

void CalculatorWidget::initUi()
{
    auto pLayout = new QGridLayout;
    const char* btnText[20] = {
        "7", "8", "9", "+", "(",
        "4", "5", "6", "-", ")",
        "1", "2", "3", "*", "<-",
        "0", ".", "=", "/", "C",
    };

    m_edit.setAlignment(Qt::AlignRight);
    m_edit.setReadOnly(true);

    pLayout->addWidget(&m_edit, 0, 0, 1, 5);

    for(int i=0; i<4; i++) {
        for(int j=0; j<5; j++) {
            m_buttons[i*5 + j].setText(btnText[i*5 + j]);
            m_buttons[i*5 + j].setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            pLayout->addWidget(&m_buttons[i*5 + j], i+1, j);

            if ((i*5 + j) == 3 || (i*5 + j) == 4 || (i*5 + j) == 8 || (i*5 + j) == 9 || (i*5 + j) == 13 || (i*5 + j) == 14 || (i*5 + j) == 18 || (i*5 + j) == 19)
                m_buttons[i*5 + j].setStyleSheet("color:white; border-radius: 5px; background-color:rgb(16, 63, 145); font: normal bold 35px;outline: none;");
            else if ((i*5 + j) == 17)
                m_buttons[i*5 + j].setStyleSheet("color:white; border-radius: 5px; background-color:rgb(71, 177, 233); font: normal bold 35px;outline: none;");
            else
                m_buttons[i*5 + j].setStyleSheet("color:white; border-radius: 5px; background-color:rgb(55, 60, 66); font: normal bold 35px;outline: none;");

            connect(&m_buttons[i*5 + j], &QPushButton::clicked, this, &CalculatorWidget::buttonClicked);
        }
    }

    pLayout->setSpacing(10);
    pLayout->setMargin(10);
    setLayout(pLayout);
    CommonHelper::setStyleSheet(":/misc/calculatorwidget/style/default.qss", this);
}

void CalculatorWidget::initCtrl()
{

}

bool CalculatorWidget::isDigitOrDot(QChar c)
{
    return  ( (('0' <= c) && (c <= '9')) || (c == '.') );
}

bool CalculatorWidget::isSymbol(QChar c)
{
    return (isOperator(c) || (c == '(') || (c == ')'));
}

bool CalculatorWidget::isSign(QChar c)
{
    return ( (c == '+') || (c == '-') );
}

bool CalculatorWidget::isNumber(QString s)
{
    bool ret = false;

    s.toDouble(&ret);

    return ret;
}

bool CalculatorWidget::isOperator(QString s)
{
    return ( (s == "+") || (s == "-") || (s == "*") || (s == "/") );
}

bool CalculatorWidget::isLeft(QString s)
{
    return (s == "(");
}

bool CalculatorWidget::isRight(QString s)
{
    return (s == ")");
}

int CalculatorWidget::priority(QString s)
{
    int ret = 0;

    if( (s == "+") || (s == "-") ) {
        ret = 1;
    }

    if( (s == "*") || (s == "/") ) {
        ret = 2;
    }

    return ret;
}

QQueue<QString> CalculatorWidget::split(const QString& exp)
{
    QQueue<QString> ret;
    QString num = "";
    QString pre = "";

    for(int i=0; i<exp.length(); i++) {
        if( isDigitOrDot(exp[i]) ) {
            num += exp[i];
            pre = exp[i];
        }
        else if( isSymbol(exp[i]) ) {
            if( !num.isEmpty() ) {
                ret.enqueue(num);
                num.clear();
            }

            if( isSign(exp[i]) && ( (pre == "") || (pre == "(") || (isOperator(pre)) ) ) {
                num += exp[i];
            }
            else {
                ret.enqueue(exp[i]);
            }

            pre = exp[i];
        }
    }

    if( !num.isEmpty() ) {
        ret.enqueue(num);
    }

    return ret;
}

bool CalculatorWidget::match(const QQueue<QString>& exp)
{
    bool ret = true;
    QStack<QString> stack;

    for(int i=0; ret && (i<exp.length()); i++) {
        if( isLeft(exp[i]) ) {
            stack.push(exp[i]);
        }
        else if( isRight(exp[i]) ) {
            if( !stack.isEmpty() && isLeft(stack.top()) ) {
                stack.pop();
            }
            else {
                ret = false;
            }
        }
    }

    return ret && stack.isEmpty();
}

bool CalculatorWidget::transform(QQueue<QString>& exp, QQueue<QString>& output)
{
    bool ret = match(exp);
    QStack<QString> stack;

    output.clear();

    while ( ret && !exp.isEmpty() ) {
        QString e = exp.dequeue();

        if( isNumber(e) ) {
            output.enqueue(e);
        }
        else if( isOperator(e) ) {
            while( (!stack.isEmpty()) && (priority(e) <= priority(stack.top())) ) {
                output.enqueue(stack.pop());
            }

            stack.push(e);
        }
        else if( isLeft(e) ) {
            stack.push(e);
        }
        else if( isRight(e) ) {
            while( !stack.isEmpty() && !isLeft(stack.top()) ) {
                output.enqueue(stack.pop());
            }

            if( !stack.isEmpty() ) {
                stack.pop();
            }
        }
        else {
            ret = false;
        }
    }

    while( !stack.isEmpty() ) {
        output.enqueue(stack.pop());
    }

    if( !ret ) {
        output.clear();
    }

    return ret;
}

QString CalculatorWidget::calculator(QString l, QString op, QString r)
{
    QString ret = "Error";

    if( isNumber(l) && isNumber(r) ) {
        double lp = l.toDouble();
        double rp = r.toDouble();

        if( op == "+" ) {
            ret.sprintf("%f", lp + rp);
        }
        else if( op == "-" ) {
            ret.sprintf("%f", lp - rp);
        }
        else if( op == "*" ) {
            ret.sprintf("%f", lp * rp);
        }
        else if( op == "/" ) {
            const double p = 0.00000001;
            if( !((-p < rp) && (rp < p) )) {
                ret.sprintf("%f", lp / rp);
            }
        }
    }

    return ret;
}

QString CalculatorWidget::calculator(QQueue<QString>& exp)
{
    QString ret = "Error";
    QStack<QString> stack;

    while( !exp.isEmpty() ) {
        QString e = exp.dequeue();

        if( isNumber(e) ) {
            stack.push(e);
        }
        else if( isOperator(e) ) {
            QString r = !stack.isEmpty() ? stack.pop() : "";
            QString l = !stack.isEmpty() ? stack.pop() : "";
            QString result = calculator(l, e, r);

            if( result != "Error" ) {
                stack.push(result);
            }
            else {
                break;
            }
        }
        else {
            break;
        }
    }

    if( exp.isEmpty() && (stack.size() == 1) && isNumber(stack.top()) ) {
        ret = stack.pop();
    }

    return ret;
}

QString CalculatorWidget::expression(const QString& exp)
{
    QString ret = "Error";
    QQueue<QString> spExp = split(exp);
    QQueue<QString> postExp;

    if( transform(spExp, postExp) ) {
        ret = calculator(postExp).replace(QRegExp("(\\.){0,1}0+$"), "");;
    }

   return ret;
}


void CalculatorWidget::buttonClicked()
{
    QPushButton* btn = dynamic_cast <QPushButton*>(sender());
    QString clickText = btn->text();

    if( btn != nullptr ) {
        if( clickText == "<-" ) {
            QString text = m_edit.text();

            if( text.length() > 0 ) {
                text.remove(text.length()-1, 1);
                m_edit.setText(text);
            }
        }
        else if( clickText == "C" ) {
            m_edit.setText("");
        }
        else if( clickText == "=" ) {
            m_edit.setText(expression(m_edit.text()));
        }
        else {
            m_edit.setText(m_edit.text() + clickText);
        }
    }
}
