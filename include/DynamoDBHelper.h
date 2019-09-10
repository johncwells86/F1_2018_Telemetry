#ifndef F1_2018_TELEMETRY_DYNAMODBHELPER_H
#define F1_2018_TELEMETRY_DYNAMODBHELPER_H

#include <aws/core/Aws.h>
#include <aws/core/utils/Outcome.h>
#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/model/AttributeDefinition.h>
#include <aws/dynamodb/model/PutItemRequest.h>
#include <aws/dynamodb/model/PutItemResult.h>
#include <iostream>


class DynamoDBHelper {

public:
    DynamoDBHelper();
    ~DynamoDBHelper() {
        Aws::ShutdownAPI(options);
    }


private:
    Aws::SDKOptions options;
    void Init();
};


#endif //F1_2018_TELEMETRY_DYNAMODBHELPER_H
