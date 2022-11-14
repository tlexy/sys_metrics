#ifndef NETWORD_DEVICES_H
#define NETWORD_DEVICES_H

#include <stdint.h>

class NetworkDevices
{
public:
    bool ok;
    // 6-chars interfaces name
    char name[7];
    // The sample time in ms.
    int64_t sample_time;

    static NetworkDevices* get_network_devices();
    static int get_network_devices_count();
    void update_network_devices();
    
public:
    // data for receive.
    unsigned long long rbytes;
    unsigned long rpackets;
    unsigned long rerrs;
    unsigned long rdrop;
    unsigned long rfifo;
    unsigned long rframe;
    unsigned long rcompressed;
    unsigned long rmulticast;
    
    // data for transmit
    unsigned long long tbytes;
    unsigned long tpackets;
    unsigned long terrs;
    unsigned long tdrop;
    unsigned long tfifo;
    unsigned long tcolls;
    unsigned long tcarrier;
    unsigned long tcompressed;

    unsigned long long diff_rbytes;
    unsigned long long diff_tbytes;
    
public:
    NetworkDevices();
};

#endif