#ifndef METRICS_UTILITY_H
#define METRICS_UTILITY_H

#include <stdint.h>

class CpuInfo
{
public:
    // Whether the data is ok.
    bool ok;
    // data of /proc/cpuinfo
public:
    // The number of processors configured.
    int nb_processors;
    // The number of processors currently online (available).
    int nb_processors_online;
    
public:
    CpuInfo();
};

class MemInfo
{
public:
    // Whether the data is ok.
    bool ok;
    // The time in ms when sample.
    int64_t sample_time;
    // The percent of usage. 0.153 is 15.3%.
    float percent_ram;
    float percent_swap;
    
    // data of /proc/meminfo
public:
    // MemActive = MemTotal - MemFree
    uint64_t MemActive;
    // RealInUse = MemActive - Buffers - Cached
    uint64_t RealInUse;
    // NotInUse = MemTotal - RealInUse
    //          = MemTotal - MemActive + Buffers + Cached
    //          = MemTotal - MemTotal + MemFree + Buffers + Cached
    //          = MemFree + Buffers + Cached
    uint64_t NotInUse;
    
    unsigned long MemTotal;
    unsigned long MemFree;
    unsigned long Buffers;
    unsigned long Cached;
    unsigned long SwapTotal;
    unsigned long SwapFree;
    
public:
    MemInfo();
};

CpuInfo* get_cpuinfo();
MemInfo* get_meminfo();
void update_meminfo();

#endif