#include "network_devices.h"
#include <string.h>
#include <stdio.h>
#include <iostream>

#include<sys/time.h>
#include <unistd.h>

#define MAX_NETWORK_DEVICES_COUNT 16
static NetworkDevices _system_network_devices[MAX_NETWORK_DEVICES_COUNT];
static int _nb_system_network_devices = -1;

static struct timeval gtv;

NetworkDevices::NetworkDevices()
{
    ok = false;
    
    memset(name, 0, sizeof(name));
    sample_time = 0;
    
    rbytes = 0;
    rpackets = 0;
    rerrs = 0;
    rdrop = 0;
    rfifo = 0;
    rframe = 0;
    rcompressed = 0;
    rmulticast = 0;
    
    tbytes = 0;
    tpackets = 0;
    terrs = 0;
    tdrop = 0;
    tfifo = 0;
    tcolls = 0;
    tcarrier = 0;
    tcompressed = 0;
}

NetworkDevices* NetworkDevices::get_network_devices()
{
    return _system_network_devices;
}

int NetworkDevices::get_network_devices_count()
{
    return _nb_system_network_devices;
}

void NetworkDevices::update_network_devices()
{
#ifndef SRS_OSX
    if (true) {
        FILE* f = fopen("/proc/net/dev", "r");
        if (f == NULL) {
            printf("open proc network devices failed, ignore");
            return;
        }
        
        // ignore title.
        static char buf[1024];
        fgets(buf, sizeof(buf), f);
        fgets(buf, sizeof(buf), f);
        
        for (int i = 0; i < MAX_NETWORK_DEVICES_COUNT; i++) {
            if (!fgets(buf, sizeof(buf), f)) {
                break;
            }
            
            NetworkDevices& r = _system_network_devices[i];
            auto rbytes = r.rbytes;
            auto tbytes = r.tbytes;
            auto last = r.sample_time;
            char fname[7];
            sscanf(buf, "%6[^:]:%llu %lu %lu %lu %lu %lu %lu %lu %llu %lu %lu %lu %lu %lu %lu %lu\n",
                   fname, &r.rbytes, &r.rpackets, &r.rerrs, &r.rdrop, &r.rfifo, &r.rframe, &r.rcompressed, &r.rmulticast,
                   &r.tbytes, &r.tpackets, &r.terrs, &r.tdrop, &r.tfifo, &r.tcolls, &r.tcarrier, &r.tcompressed);
            
            sscanf(fname, "%s", r.name);
            _nb_system_network_devices = i + 1;
            //printf("scan network device ifname=%s, total=%d", r.name, _nb_system_network_devices);
            r.ok = true;

            //统计处理
            if (rbytes > 0)
            {
                r.diff_rbytes = r.rbytes - rbytes;
            }
            if (tbytes > 0)
            {
                r.diff_tbytes = r.tbytes - tbytes;
            }
            gettimeofday(&gtv, NULL);
		    r.sample_time = gtv.tv_sec * 1000 + gtv.tv_usec / 1000;

            double tperd = r.diff_tbytes/1.0/(r.sample_time - last);
            int tper = tperd*1000;

            double rperd = r.diff_rbytes/1.0/(r.sample_time - last);
            int rper = rperd*1000;
            printf("tbytes/s: %d, rbytes/s: %d\n", tper, rper);
        }
        
        fclose(f);
    }
#endif
}