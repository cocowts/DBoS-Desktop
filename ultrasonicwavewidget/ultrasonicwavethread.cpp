#include "ultrasonicwavethread.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <QDebug>

UltrasonicwaveThread::UltrasonicwaveThread(const QString &path) : QObject(NULL)
{
    m_path = path;

    moveToThread(&m_thread);

    connect(&m_thread, &QThread::started, this, &UltrasonicwaveThread::tmain);
    connect(&m_thread, &QThread::finished, this, &UltrasonicwaveThread::finished);
}

UltrasonicwaveThread::~UltrasonicwaveThread()
{
}

bool UltrasonicwaveThread::start()
{
    m_fd = ::open(m_path.toLatin1().data(), O_RDONLY);

    if (m_fd == -1) {
        return false;
    }

    m_thread.start();

    return true;
}

void UltrasonicwaveThread::quit()
{
    m_thread.requestInterruption();
    m_thread.quit();
}

void UltrasonicwaveThread::wait()
{
    m_thread.wait();
}

void UltrasonicwaveThread::tmain()
{
    int value = 0;
    while (!QThread::currentThread()->isInterruptionRequested()) {

        if (::read(m_fd, &value, 4) == 4) {
            emit readReady(value);
        }
        else {
            emit readReady(-1);
        }

        m_thread.msleep(300);
    }

    ::close(m_fd);
}
