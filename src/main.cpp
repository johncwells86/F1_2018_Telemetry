#include <thread>
#include "../include/UDPListener.h"
#include "../include/SafeQueue.h"
#include "../include/PacketAnalyzer.h"
#include "../include/packets.h"
int main(int argc, char *argv[])
{
    SafeQueue<NetworkPacket>* q = new SafeQueue<NetworkPacket>();
    PacketAnalyzer* packet_analyzer = new PacketAnalyzer(q);
    UDPListener* udp = new UDPListener(20777, q);
    std::thread uThread([udp] {
        udp->run();
    });

    std::thread pThread([packet_analyzer]{ 
        packet_analyzer->run();
    });
    pThread.join();
    // udp.run();
    return 0;
}
