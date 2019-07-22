#ifndef UDPLISTENER_H
#define UDPLISTENER_H

#include <string>
#include <sys/socket.h> 
#include <future>
#include "packets.h"
#include "SafeQueue.h"

// auto PACKET_SIZE = 2048;

class UDPListener {

public:
  	UDPListener(int port, SafeQueue<NetworkPacket>* q);
  	~UDPListener();
  	void run();

private:
    int socket;
    std::string ip_addr;
    uint16_t port;
  	SafeQueue<NetworkPacket>* q;
};

#endif
