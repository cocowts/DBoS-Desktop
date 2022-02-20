#include "dynamicline.h"

#include <QDateTime>
#include <QStringLiteral>
#include <QVBoxLayout>

DynamicLine::DynamicLine(QWidget *parent)
    : QWidget(parent), m_pChartView(new QChartView(this)), m_pChart(new QChart), m_pTimeAxisX(new QDateTimeAxis(this)), m_pAxisY(new QValueAxis(this))
{
    initUi();
    initCtrl();
}

DynamicLine::~DynamicLine()
{

}

void DynamicLine::initUi()
{
    m_pTimeAxisX->setLabelsAngle(45);
    m_pChart->setParent(m_pChartView);
    m_pChart->addAxis(m_pTimeAxisX, Qt::AlignBottom);
    m_pChart->addAxis(m_pAxisY,     Qt::AlignLeft);
    m_pChart->setTheme(QChart::ChartThemeDark);
    m_pChart->setAnimationOptions(QChart::SeriesAnimations);
    m_pChart->legend()->setAlignment(Qt::AlignBottom);
    m_pChart->legend()->setVisible(false);  // ！！

    m_pChartView->setChart(m_pChart);
    m_pChartView->setRenderHint(QPainter::Antialiasing);
    m_pChartView->setRubberBand(QChartView::RectangleRubberBand);
    m_pChartView->installEventFilter(this);

    QFont font("微软雅黑", 12);
    font.setBold(true);

    m_pChart->setTitleFont(font);

    font.setPointSize(9);
    m_pTimeAxisX->setLabelsFont(font);
    m_pAxisY->setLabelsFont(font);

    font.setPointSize(10);
    m_pChart->legend()->setFont(font);

    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget(m_pChartView);
    pLayout->setContentsMargins(0, 0, 0, 0);
    pLayout->setSpacing(0);
    setLayout(pLayout);
}

QLineSeries *DynamicLine::createSeries(QChart *chart, const QString &name, const QPen &pen, bool pointsVisible)
{
    QLineSeries *ret = new QLineSeries(chart);

    chart->addSeries(ret);

    ret->setName(name);
    ret->attachAxis(m_pTimeAxisX);
    ret->attachAxis(m_pAxisY);
    ret->setPointsVisible(pointsVisible);
    ret->setPen(pen);

    return ret;
}

void DynamicLine::initCtrl()
{
    connect(&m_timer, &QTimer::timeout, this, &DynamicLine::updateChart);
}

void DynamicLine::start(int intervalMsec)
{
    if (m_timer.isActive())
        m_timer.stop();

    m_timer.start(intervalMsec);
}

void DynamicLine::stop()
{
    m_timer.stop();
}

int DynamicLine::count() const
{
    return m_series.count();
}

qint64 DynamicLine::timeAxisXSpanSecs() const
{
    return m_setSpanSecs;
}

void DynamicLine::setSeriesVisible(int index, bool visible)
{
    if (index < m_series.count())
        m_series[index]->setVisible(visible);
}

void DynamicLine::setChartTitle(const QString &title)
{
    m_pChart->setTitle(title);
}

void DynamicLine::setSeriesValues(int index, double value)
{
    if (index < m_lastValues.count())
        m_lastValues[index] = value;
}

void DynamicLine::addSplineSeries(const QString &name, const QPen &pen, bool pointsVisible)
{
    QLineSeries *series = new QLineSeries(m_pChart);

    m_pChart->addSeries(series);

    series->setName(name);
    series->attachAxis(m_pTimeAxisX);
    series->attachAxis(m_pAxisY);
    series->setPointsVisible(pointsVisible);
    series->setPen(pen);

    m_series.append(series);
    m_lastValues.append(0);
}

void DynamicLine::setTimeAxisXSpanSecs(qint64 secs)
{
    m_setSpanSecs = secs;

    m_curSpanSecs = m_setSpanSecs;
}

void DynamicLine::setTimeAxisXFormat(const QString &format)
{
    m_pTimeAxisX->setFormat(format);
}

void DynamicLine::setTimeAxisXTickCount(int tickCount)
{
    m_pTimeAxisX->setTickCount(tickCount);
}

void DynamicLine::setAxisYRange(qreal min, qreal max)
{
    m_axisRange.max = max;
    m_axisRange.min = min;

    m_pAxisY->setRange(min, max);
}

void DynamicLine::setpAxisYTickCount(int count)
{
    m_pAxisY->setTickCount(count);
}

void DynamicLine::setAxisYLabelFormat(const QString &format)
{
    m_pAxisY->setLabelFormat(format);
}

void DynamicLine::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_pAxisY->setRange(m_axisRange.min, m_axisRange.max);

        m_curSpanSecs = m_setSpanSecs;

        QDateTime endTime   = QDateTime::currentDateTime();;
        QDateTime beginTime = endTime.addSecs(-m_setSpanSecs);
        m_pTimeAxisX->setMin(beginTime);
        m_pTimeAxisX->setMax(endTime);
    }
}

bool DynamicLine::eventFilter(QObject *o, QEvent *e)
{
    if ((o == qobject_cast<QObject *>(m_pChartView)) && (e->type() == QEvent::LayoutRequest))
    {
        m_curSpanSecs = m_pTimeAxisX->max().toSecsSinceEpoch() - m_pTimeAxisX->min().toSecsSinceEpoch();
    }

    return false;
}

void DynamicLine::updateChart()
{
    QDateTime dateTime = QDateTime::currentDateTime();

    if (!m_isFirstTime)
    {
        m_isFirstTime = true;

        QDateTime beginTime = dateTime;
        QDateTime endTime = beginTime.addSecs(m_curSpanSecs);
        m_pTimeAxisX->setMin(beginTime);
        m_pTimeAxisX->setMax(endTime);
    }

    for (int i=0; i<m_series.count(); ++i)
    {
        m_series[i]->append(QDateTime::currentMSecsSinceEpoch(), m_lastValues.at(i));
    }

    if (m_pTimeAxisX->max() < dateTime)
    {
        QDateTime endTime   = dateTime;
        QDateTime beginTime = endTime.addSecs(-m_curSpanSecs);
        m_pTimeAxisX->setMin(beginTime);
        m_pTimeAxisX->setMax(endTime);

        int pointCnt = m_setSpanSecs *  (1000 / m_timer.interval());
        int totalPointCnt = pointCnt * 8;

        for (auto &series : m_series)
        {
             if (series->count() > totalPointCnt)
             {
                 series->removePoints(0, totalPointCnt - pointCnt);
             }
        }
    }
}

