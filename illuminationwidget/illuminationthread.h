#ifndef ILLUMINATIONTHREAD_H
#define ILLUMINATIONTHREAD_H

#include <QObject>
#include <QThread>

class IlluminationThread : public QObject
{
    Q_OBJECT
public:
    explicit IlluminationThread(const QString &path);
    ~IlluminationThread();

public slots:
    bool start();
    void quit();
    void wait();

private slots:
    void tmain();

signals:
    void readReady(unsigned short ir, unsigned short als, unsigned short ps);
    void finished();

private:
    QString m_path;
    QThread m_thread;
    int m_fd = -1;
};

#endif
