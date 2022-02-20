#ifndef SLIDERWIDGET_H
#define SLIDERWIDGET_H

#include <QHBoxLayout>
#include <QPropertyAnimation>
#include <QScrollArea>
#include <QScroller>
#include <QTimer>
#include <QWidget>

class SliderWidget : public QWidget
{
    Q_OBJECT

public:
    SliderWidget(QWidget *parent = nullptr);
    ~SliderWidget();

    int addWidget(QWidget *page);
    int count() const;
    int currentIndex() const;
    QWidget *currentWidget() const;
    int indexOf(QWidget *widget) const;

public slots:
    void setCurrentIndex(int index);
    void setCurrentWidget(QWidget *widget);

    void removeWidget(int index);
    void removeWidget(QWidget *widget);

protected:
    void resizeEvent(QResizeEvent *ev) override;

protected slots:
    void onScrollerStateChanged(QScroller::State state);
    void onSliderTimerTimeout();

signals:
    void currentChanged(int index);

private:
    void initUi();
    void initCtrl();
    void updateIndicator(int index);

private:
    QWidget *m_pMainWidget;
    QScrollArea *m_pScrollArea;
    QHBoxLayout *m_pMainLayout;
    QWidget *m_pIndicatorWidget;
    QHBoxLayout *m_pIndicatorLayout;

    QPropertyAnimation *m_pAnimation;
    QTimer *m_pSlidingTimer;

    bool m_sliderFlag = false;
    int m_sliderPressedValue = 0;
    int m_sliderReleaseValue = 0;
    int m_curPageIndex = -1;
    int m_nextPageIndex = -1;
};

#endif // SLIDERWIDGET_H
