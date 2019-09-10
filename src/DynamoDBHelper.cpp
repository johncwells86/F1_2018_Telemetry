#include "DynamoDBHelper.h"

/**
* Put an item in a DynamoDB table.
*
* Takes the name of the table, a name (primary key value) and a greeting
* (associated with the key value).
*
* This code expects that you have AWS credentials set up per:
* http://docs.aws.amazon.com/sdk-for-cpp/v1/developer-guide/credentials.html
*/
DynamoDBHelper::DynamoDBHelper() {
    Aws::InitAPI(options);
}

void DynamoDBHelper::Push(const json& j, const std::string& table)
{
//    const std::string USAGE = \
//        "Usage:\n"
//        "    put_item <table> <name> [field=value ...]\n\n"
//        "Where:\n"
//        "    table    - the table to put the item in.\n"
//        "    name     - a name to add to the table. If the name already\n"
//        "               exists, its entry will be updated.\n\n"
//        "Additional fields can be added by appending them to the end of the\n"
//        "input.\n\n"
//        "Example:\n"
//        "    put_item Cellists Pau Language=ca Born=1876\n";


    const Aws::String table(argv[1]);
    const Aws::String name(argv[2]);

    // snippet-start:[dynamodb.cpp.put_item.code]
    Aws::Client::ClientConfiguration clientConfig;
    Aws::DynamoDB::DynamoDBClient dynamoClient(clientConfig);

    Aws::DynamoDB::Model::PutItemRequest pir;
    pir.SetTableName(table);

    Aws::DynamoDB::Model::AttributeValue av;
    av.SetS(name);
    pir.AddItem("Name", av);

    for (int x = 3; x < argc; x++)
    {
        const Aws::String arg(argv[x]);
        const Aws::Vector<Aws::String>& flds = Aws::Utils::StringUtils::Split(arg, '=');
        if (flds.size() == 2)
        {
            Aws::DynamoDB::Model::AttributeValue val;
            val.SetS(flds[1]);
            pir.AddItem(flds[0], val);
        }
        else
        {
            std::cout << "Invalid argument: " << arg << std::endl << USAGE;
            return 1;
        }
    }

    const Aws::DynamoDB::Model::PutItemOutcome result = dynamoClient.PutItem(pir);

    // snippet-end:[dynamodb.cpp.put_item.code]
}