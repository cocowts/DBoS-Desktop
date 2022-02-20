#include "drawwidget.h"

#include <QPainter>

DrawWidget::DrawWidget(QWidget *parent) : QWidget(parent)
{ }

void DrawWidget::mousePressEvent(QMouseEvent* e)
{
    m_points.append(e->pos());
}

void DrawWidget::mouseMoveEvent(QMouseEvent* e)
{
    m_points.append(e->pos());

    update();
}

void DrawWidget::mouseReleaseEvent(QMouseEvent* e)
{
    m_points.append(e->pos());
    m_drawlist.append(m_points);
    m_points.clear();
    update();
}

void DrawWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    painter.setPen("white");

    for(int i=0; i<m_drawlist.count(); i++) {
        for(int j=0; j<m_drawlist[i].count()-1; j++) {
            painter.drawLine(m_drawlist[i][j], m_drawlist[i][j+1]);
        }
    }

    for(int i=0; i<m_points.count()-1; i++) {
        painter.drawLine(m_points[i], m_points[i+1]);
    }
}

void DrawWidget::clear()
{
    m_drawlist.clear();
    update();
}

QList<QList<QPoint>>* DrawWidget::getDraw()
{
    return &m_drawlist;
}
