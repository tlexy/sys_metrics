#include "proc_system_stat.h"
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ProcSystemStat ProcSystemStat::_cpu_system_stat;

bool get_proc_system_stat(ProcSystemStat& r)
{
#ifndef SRS_OSX
    FILE* f = fopen("/proc/stat", "r");
    if (f == NULL) {
        printf("open system cpu stat failed, ignore");
        return false;
    }
    
    static char buf[1024];
    while (fgets(buf, sizeof(buf), f)) {
        if (strncmp(buf, "cpu ", 4) != 0) {
            continue;
        }
        
        // @see: read_stat_cpu() from https://github.com/sysstat/sysstat/blob/master/rd_stats.c#L88
        // @remark, ignore the filed 10 cpu_guest_nice
        sscanf(buf + 5, "%llu %llu %llu %llu %llu %llu %llu %llu %llu\n",
               &r.user,
               &r.nice,
               &r.sys,
               &r.idle,
               &r.iowait,
               &r.irq,
               &r.softirq,
               &r.steal,
               &r.guest);
        
        break;
    }
    
    fclose(f);
#endif

    r.ok = true;
    
    return true;
}


void ProcSystemStat::update_proc_stat()
{
    // @see: http://stackoverflow.com/questions/7298646/calculating-user-nice-sys-idle-iowait-irq-and-sirq-from-proc-stat/7298711
    // @see https://github.com/ossrs/srs/issues/397
    static int user_hz = 0;
    if (user_hz <= 0) {
        user_hz = (int)sysconf(_SC_CLK_TCK);
        printf("USER_HZ=%d\n", user_hz);
    }
    
    // system cpu stat
    if (true) {
        ProcSystemStat r;
        if (!get_proc_system_stat(r)) {
            return;
        }
        
        // calc usage in percent
        ProcSystemStat& o = _cpu_system_stat;
        
        // @see: http://blog.csdn.net/nineday/article/details/1928847
        // @see: http://stackoverflow.com/questions/16011677/calculating-cpu-usage-using-proc-files
        if (o.total() > 0) {
            r.total_delta = r.total() - o.total();
        }
        if (r.total_delta > 0) {
            int64_t idle = r.idle - o.idle;
            r.percent = (float)(1 - idle / (double)r.total_delta);
            printf("cpu percent: %0.2f\n", r.percent);
        }
        
        // upate cache.
        _cpu_system_stat = r;
    }
}

ProcSystemStat* ProcSystemStat::get_system_proc_stat()
{
    return &_cpu_system_stat;
}

ProcSystemStat::ProcSystemStat()
{
    ok = false;
    sample_time = 0;
    percent = 0;
    total_delta = 0;
    user = 0;
    nice = 0;
    sys = 0;
    idle = 0;
    iowait = 0;
    irq = 0;
    softirq = 0;
    steal = 0;
    guest = 0;
}

int64_t ProcSystemStat::total()
{
    return user + nice + sys + idle + iowait + irq + softirq + steal + guest;
}