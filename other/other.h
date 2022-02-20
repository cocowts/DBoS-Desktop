#ifndef OTHER_H
#define OTHER_H

#include <QObject>

class Other : public QObject
{
    Q_OBJECT

    struct CpuOccupy {
        char name[20];
        unsigned int user;      /* 从系统启动开始累计到当前时刻，处于用户态的运行时间，不包含 nice 值为负进程 */
        unsigned int nice;      /* 从系统启动开始累计到当前时刻，nice 值为负的进程作占用的 CPU 时间 */
        unsigned int system;    /* 从系统启动开始累计到当前时刻，处于和心态的运行时间 */
        unsigned int idle;      /* 从系统启动开始累计到当前时刻，除IO等待时间以外的其他等待时间 */
        unsigned int iowait;    /* 从系统启动开始累计到当前时刻，IO等待时间 */
        unsigned int irq;       /* 从系统启动开始累计到当前时刻，硬中断时间 */
        unsigned int softirq;   /* 从系统启动开始累计到当前时刻，软中断时间 */
    };

public:
    Other(QObject *parent = nullptr);

    double getSysCpuUsage();
    double getCpuTemperature();

private:
    void getCpuOccupy(CpuOccupy &cpust);

private:
    CpuOccupy m_preCpuOccupy;
};

#endif // OTHER_H
