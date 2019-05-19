#ifndef PACKETANALYZER_H
#define PACKETANALYZER_H

#include "packets.h"
#include <future>
#include "SafeQueue.h"

class PacketAnalyzer {
public:
	PacketAnalyzer(SafeQueue<NetworkPacket>* q): q(q) 
	{

	}
	~PacketAnalyzer() {}
 	void run();
  
private:
  	SafeQueue<NetworkPacket>* q;

  	// void packetAnalyzed(QPointF throttle, QPointF brake, QPointF steer);

};

#endif
