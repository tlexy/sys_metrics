#include "proc_system_stat.h"
#include "network_devices.h"
#include "metrics_utility.h"
#include <thread>

//cat /proc/net/dev
//cat /proc/net/snmp
//cat /sys/class/net/eth0

static ProcSystemStat* pss = nullptr;
static NetworkDevices* nd = nullptr;

void timer()
{
    CpuInfo* cpu_info = get_cpuinfo();
    printf("numbers of cpu: %d\n", cpu_info->nb_processors);
    pss = new ProcSystemStat();
    nd = new NetworkDevices();

    pss->update_proc_stat();
    nd->update_network_devices();
    while (true)
    {
        if (pss)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            pss->update_proc_stat();
            nd->update_network_devices();

            update_meminfo();
            MemInfo* mi = get_meminfo();
            printf("mem active: %lld, mem in use: %lld, mem not use: %lld\n", mi->MemActive, mi->RealInUse, mi->NotInUse);
        }
    }
}

int main()
{
    std::thread th(timer);
    th.join();    
}