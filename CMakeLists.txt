cmake_minimum_required(VERSION 3.14)
project(F1_2018_Telemetry)

set(CMAKE_CXX_STANDARD 14)
add_subdirectory(external/aws-sdk-cpp)
find_package(AWSSDK REQUIRED COMPONENTS dynamodb)

#set(aws-sdk-cpp_DIR ${CMAKE_ROOT}/external/aws-sdk-cpp/*)
#set(BUILD_ONLY dynamodb)
include_directories(external/json)
include_directories(external/json/single_include)
include_directories(external/json/single_include/nlohmann)
include_directories(external/aws-sdk-cpp)
include_directories(external/aws-sdk-cpp/aws-cpp-sdk-core/include)
include_directories(external/aws-sdk-cpp/aws-cpp-sdk-dynamodb/include)

#target_link_libraries(${EXAMPLE} ${AWSSDK_LINK_LIBRARIES})

include_directories(include)
add_executable(f1
#    external/json/single_include/nlohmann/json.hpp
#    external/json/CMakeLists.txt
    include/PacketAnalyzer.h
    include/packets.h
    include/SafeQueue.h
    include/UDPListener.h
    src/main.cpp
    src/PacketAnalyzer.cpp
    src/UDPListener.cpp src/DynamoDBHelper.cpp include/DynamoDBHelper.h)
