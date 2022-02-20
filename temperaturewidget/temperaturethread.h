#ifndef TEMPERATURETHREAD_H
#define TEMPERATURETHREAD_H

#include <QObject>
#include <QThread>

class TemperatureThread : public QObject
{
    Q_OBJECT
public:
    explicit TemperatureThread(const QString &path);
    ~TemperatureThread();

public slots:
    bool start();
    void quit();
    void wait();

private slots:
    void tmain();

signals:
    void readReady(double temp, double humi);
    void finished();

private:
    QString m_path;
    QThread m_thread;
    int m_fd = -1;
};

#endif
