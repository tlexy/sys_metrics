#ifndef PROC_SYSTEM_STAT_TX_H
#define PROC_SYSTEM_STAT_TX_H

#include <limits.h>
#include <arpa/inet.h>
#include <sys/resource.h>
#include <stdint.h>

class ProcSystemStat
{
public:
    // Whether the data is ok.
    bool ok;
    // The time in ms when sample.
    int64_t sample_time;
    // The percent of usage. 0.153 is 15.3%.
    // The percent is in [0, 1], where 1 is 100%.
    // for multiple core cpu, max also is 100%.
    float percent;
    // The total cpu time units
    // @remark, zero for the previous total() is zero.
    //          the usaged_cpu_delta = total_delta * percent
    //          previous cpu total = this->total() - total_delta
    int64_t total_delta;
    
    // data of /proc/stat
public:
    // The amount of time, measured in units  of  USER_HZ
    // (1/100ths  of  a  second  on  most  architectures,  use
    // sysconf(_SC_CLK_TCK)  to  obtain  the  right value)
    //
    // The system spent in user mode,
    unsigned long long user;
    // user mode with low priority (nice),
    unsigned long long nice;
    // system mode,
    unsigned long long sys;
    // and the idle task, respectively.
    unsigned long long idle;
    
    // In  Linux 2.6 this line includes three additional columns:
    //
    // iowait - time waiting for I/O to complete (since 2.5.41);
    unsigned long long iowait;
    // irq - time servicing interrupts (since 2.6.0-test4);
    unsigned long long irq;
    // softirq  -  time  servicing  softirqs  (since 2.6.0-test4).
    unsigned long long softirq;
    
    // Since  Linux 2.6.11, there is an eighth column,
    // steal - stolen time, which is the time spent in other oper-
    // ating systems when running in a virtualized environment
    unsigned long long steal;
    
    // Since Linux 2.6.24, there is a ninth column,
    // guest, which is the time spent running a virtual CPU for guest
    // operating systems under the control of the Linux kernel.
    unsigned long long guest;
    
public:
    ProcSystemStat();
    
    ProcSystemStat* get_system_proc_stat();
    void update_proc_stat();
    // Get total cpu units.
    int64_t total();

private:
    static ProcSystemStat _cpu_system_stat;
};

#endif