#ifndef PACKETANALYZER_H
#define PACKETANALYZER_H

//#include <future>

#include <aws/core/Aws.h>
#include <aws/core/utils/Outcome.h>
#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/model/AttributeDefinition.h>
#include <aws/dynamodb/model/PutItemRequest.h>
#include <aws/dynamodb/model/PutItemResult.h>

#include <nlohmann/json.hpp>

#include "SafeQueue.h"
#include "packets.h"

class PacketAnalyzer {
public:
	PacketAnalyzer(SafeQueue<NetworkPacket>* q);
	~PacketAnalyzer() {}
 	void run();
  
private:
    Aws::Client::ClientConfiguration clientConfig;
    Aws::DynamoDB::DynamoDBClient dynamoClient;
    void pushToDynamoDB(nlohmann::json j, const std::string & table);

  	SafeQueue<NetworkPacket>* q;

  	// void packetAnalyzed(QPointF throttle, QPointF brake, QPointF steer);

};

#endif
