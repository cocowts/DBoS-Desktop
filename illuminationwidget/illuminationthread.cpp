#include "illuminationthread.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

IlluminationThread::IlluminationThread(const QString &path) : QObject(NULL)
{
    m_path = path;

    moveToThread(&m_thread);

    connect(&m_thread, &QThread::started, this, &IlluminationThread::tmain);
    connect(&m_thread, &QThread::finished, this, &IlluminationThread::finished);
}

IlluminationThread::~IlluminationThread()
{
}

bool IlluminationThread::start()
{
    m_fd = ::open(m_path.toLatin1().data(), O_RDONLY);

    if (m_fd == -1) {
        return false;
    }

    m_thread.start();

    return true;
}

void IlluminationThread::quit()
{
    m_thread.requestInterruption();
    m_thread.quit();
}

void IlluminationThread::wait()
{
    m_thread.wait();
}

void IlluminationThread::tmain()
{
    unsigned short data[3];
    while (!QThread::currentThread()->isInterruptionRequested()) {

        if (::read(m_fd, data, 6) == 6) {
            emit readReady(data[0], data[2], data[1]);
        }
        else {
            emit readReady(-1, -1, -1);
        }

        m_thread.msleep(200);
    }

    ::close(m_fd);
}
