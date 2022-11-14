#include "metrics_utility.h"
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>

CpuInfo::CpuInfo()
{
    ok = false;
    
    nb_processors = 0;
    nb_processors_online = 0;
}

CpuInfo* get_cpuinfo()
{
    static CpuInfo* cpu = NULL;
    if (cpu != NULL) {
        return cpu;
    }
    
    // initialize cpu info.
    cpu = new CpuInfo();
    cpu->ok = true;
    cpu->nb_processors = (int)sysconf(_SC_NPROCESSORS_CONF);
    cpu->nb_processors_online = (int)sysconf(_SC_NPROCESSORS_ONLN);
    
    return cpu;
}

MemInfo::MemInfo()
{
    ok = false;
    sample_time = 0;
    
    percent_ram = 0;
    percent_swap = 0;
    
    MemActive = 0;
    RealInUse = 0;
    NotInUse = 0;
    MemTotal = 0;
    MemFree = 0;
    Buffers = 0;
    Cached = 0;
    SwapTotal = 0;
    SwapFree = 0;
}

static MemInfo _system_meminfo;

MemInfo* get_meminfo()
{
    return &_system_meminfo;
}

void update_meminfo()
{
    MemInfo& r = _system_meminfo;

#ifndef SRS_OSX
    FILE* f = fopen("/proc/meminfo", "r");
    if (f == NULL) {
        printf("open meminfo failed, ignore");
        return;
    }
    
    static char buf[1024];
    while (fgets(buf, sizeof(buf), f)) {
        // @see: read_meminfo() from https://github.com/sysstat/sysstat/blob/master/rd_stats.c#L227
        if (strncmp(buf, "MemTotal:", 9) == 0) {
            sscanf(buf + 9, "%lu", &r.MemTotal);
        } else if (strncmp(buf, "MemFree:", 8) == 0) {
            sscanf(buf + 8, "%lu", &r.MemFree);
        } else if (strncmp(buf, "Buffers:", 8) == 0) {
            sscanf(buf + 8, "%lu", &r.Buffers);
        } else if (strncmp(buf, "Cached:", 7) == 0) {
            sscanf(buf + 7, "%lu", &r.Cached);
        } else if (strncmp(buf, "SwapTotal:", 10) == 0) {
            sscanf(buf + 10, "%lu", &r.SwapTotal);
        } else if (strncmp(buf, "SwapFree:", 9) == 0) {
            sscanf(buf + 9, "%lu", &r.SwapFree);
        }
    }
    fclose(f);
#endif
    r.MemActive = r.MemTotal - r.MemFree;
    r.RealInUse = r.MemActive - r.Buffers - r.Cached;
    r.NotInUse = r.MemTotal - r.RealInUse;
    
    if (r.MemTotal > 0) {
        r.percent_ram = (float)(r.RealInUse / (double)r.MemTotal);
    }
    if (r.SwapTotal > 0) {
        r.percent_swap = (float)((r.SwapTotal - r.SwapFree) / (double)r.SwapTotal);
    }
    
    r.ok = true;
}