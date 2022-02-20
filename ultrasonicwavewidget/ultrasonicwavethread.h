#ifndef ULTRASONICWAVETHREAD_H
#define ULTRASONICWAVETHREAD_H

#include <QObject>
#include <QThread>

class UltrasonicwaveThread : public QObject
{
    Q_OBJECT
public:
    explicit UltrasonicwaveThread(const QString &path);
    ~UltrasonicwaveThread();

public slots:
    bool start();
    void quit();
    void wait();

private slots:
    void tmain();

signals:
    void readReady(int value);
    void finished();

private:
    QString m_path;
    QThread m_thread;
    int m_fd = -1;
};

#endif // ULTRASONICWAVETHREAD_H
