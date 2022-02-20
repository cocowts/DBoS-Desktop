#include "other.h"

#include <QFile>

Other::Other(QObject *parent) : QObject(parent)
{

}

double Other::getSysCpuUsage()
{
    double cpuUsage = 0.0;

    CpuOccupy currCpuOccupy;

    getCpuOccupy(currCpuOccupy);

    auto sum_pre = m_preCpuOccupy.user + m_preCpuOccupy.nice + m_preCpuOccupy.system + m_preCpuOccupy.idle + m_preCpuOccupy.iowait + m_preCpuOccupy.irq + m_preCpuOccupy.softirq;
    auto sum_curr = currCpuOccupy.user + currCpuOccupy.nice + currCpuOccupy.system + currCpuOccupy.idle + currCpuOccupy.iowait + currCpuOccupy.irq + currCpuOccupy.softirq;
    if (sum_pre != sum_curr)
        cpuUsage = 100.0 - ((double)(currCpuOccupy.idle - m_preCpuOccupy.idle)  / (double)(sum_curr - sum_pre)) * 100.0;

    m_preCpuOccupy = currCpuOccupy;

    if (cpuUsage < 0.0)
        cpuUsage = 0.0;

    return cpuUsage;
}

void Other::getCpuOccupy(CpuOccupy &cpust)
{
    QFile file("/proc/stat");

    if (file.open(QIODevice::ReadOnly))
    {
        auto line = file.readLine().split(' ');

        cpust.user = line.at(2).toUInt();
        cpust.nice = line.at(3).toUInt();
        cpust.system = line.at(4).toUInt();
        cpust.idle = line.at(5).toUInt();
        cpust.iowait = line.at(6).toUInt();
        cpust.irq = line.at(7).toUInt();
        cpust.softirq = line.at(8).toUInt();

        file.close();
    }
}

double Other::getCpuTemperature()
{
    double cpuTemperature = 0.0;

    QFile file("/sys/class/hwmon/hwmon0/temp1_input");

    if (file.open(QIODevice::ReadOnly))
    {
        cpuTemperature = file.readAll().toDouble() / 1000.0;

        file.close();
    }

    return cpuTemperature;
}
