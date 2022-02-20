#include "temperaturethread.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

TemperatureThread::TemperatureThread(const QString &path) : QObject(NULL)
{
    m_path = path;

    moveToThread(&m_thread);

    connect(&m_thread, &QThread::started, this, &TemperatureThread::tmain);
    connect(&m_thread, &QThread::finished, this, &TemperatureThread::finished);
}

TemperatureThread::~TemperatureThread()
{
}

bool TemperatureThread::start()
{
    m_fd = ::open(m_path.toLatin1().data(), O_RDONLY);

    if (m_fd == -1) {
        return false;
    }

    m_thread.start();

    return true;
}

void TemperatureThread::quit()
{
    m_thread.requestInterruption();
    m_thread.quit();
}

void TemperatureThread::wait()
{
    m_thread.wait();
}

void TemperatureThread::tmain()
{
    unsigned char buf[4];
    while (!QThread::currentThread()->isInterruptionRequested()) {

        if (::read(m_fd, buf, 4) == 4) {
            emit readReady(buf[2]+(buf[3]/10.0), buf[0]+(buf[1]/10.0));
        }
        else {
            emit readReady(-1, -1);
        }

        m_thread.msleep(1500);
    }

    ::close(m_fd);
}
