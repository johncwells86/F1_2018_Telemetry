#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <chrono>
#include <nlohmann/json.hpp>
#include <mongocxx/v_noabi/mongocxx/client.hpp>

#include "../include/PacketAnalyzer.h"

using namespace nlohmann;

void PacketAnalyzer::run()
{
    std::ofstream telFile;
    telFile.open("telemetry.json", std::ios::out | std::ios::app);
    while (true) {
        struct NetworkPacket in_packet = q->pop_item();
    
        uint64_t sessionUID = in_packet.header.sessionUID;
        uint8_t packet_type = 127;
        packet_type = in_packet.header.packetId;
        float packet_time = in_packet.header.sessionTime;
        int playerCarIndex = in_packet.header.playerCarIndex;
        switch (packet_type) {
        case 0: 
        {
            
            struct PacketMotionData *motionData = (struct PacketMotionData *)&in_packet.data;
            float worldPosX = motionData->carMotionData[playerCarIndex].worldPosX;
            float worldPosY = motionData->carMotionData[playerCarIndex].worldPosY;
            float worldPosZ = motionData->carMotionData[playerCarIndex].worldPosZ;
            float worldVelX = motionData->carMotionData[playerCarIndex].worldVelX;
            float worldVelY = motionData->carMotionData[playerCarIndex].worldVelY;
            float worldVelZ = motionData->carMotionData[playerCarIndex].worldVelZ;
            int16_t worldForwardDirX = motionData->carMotionData[playerCarIndex].worldForwardDirX;
            int16_t worldForwardDirY = motionData->carMotionData[playerCarIndex].worldForwardDirY;
            int16_t worldForwardDirZ = motionData->carMotionData[playerCarIndex].worldForwardDirZ;
            int16_t worldRightDirX = motionData->carMotionData[playerCarIndex].worldRightDirX;
            int16_t worldRightDirY = motionData->carMotionData[playerCarIndex].worldRightDirY;
            int16_t worldRightDirZ = motionData->carMotionData[playerCarIndex].worldRightDirZ;
            json j = { 
                "session_time", packet_time,
                "position", {
                    { "worldPosX", worldPosX },
                    { "worldPosY", worldPosY },  
                    { "worldPosZ", worldPosZ }
                },
                "velocity", { 
                    { "worldVelX", worldVelX },
                    { "worldVelY", worldVelY },  
                    { "worldVelZ", worldVelZ },
                }, 
                "direction", { 
                    { "worldForwardDirX", worldForwardDirX },
                    { "worldForwardDirY", worldForwardDirY },  
                    { "worldForwardDirZ", worldForwardDirZ },
                    { "worldRightDirX", worldRightDirX },
                    { "worldRightDirY", worldRightDirY },  
                    { "worldRightDirZ", worldRightDirZ },
                }
            };
            telFile << j;
            break;
        }
        case 1:
        {
            struct PacketSessionData *data = (struct PacketSessionData *)&in_packet.data;
            break;
        }
        case 2:
        {
            struct PacketLapData *lapData = (struct PacketLapData *)&in_packet.data;
            break;
        }
        case 3:
        {
            break;
        }
        case 4:
        {
            break;
        }
        case 5:
        {
            struct PacketCarSetupData *setupData = (struct PacketCarSetupData *)&in_packet.data;
            break;
        }
        case 6:
        {
        // std::map<std::string, auto> mep;
            std::cout << "Telemetry" << std::endl;
            struct PacketCarTelemetryData *telemetryData = (struct PacketCarTelemetryData *)&in_packet.data;
        
            auto speed = telemetryData->carTelemetryData[0].speed;
            auto throttle = telemetryData->carTelemetryData[0].throttle;
            auto steer = telemetryData->carTelemetryData[0].steer;
            auto brake = telemetryData->carTelemetryData[0].brake; 
            auto gear = telemetryData->carTelemetryData[0].gear;
            auto engineRPM = telemetryData->carTelemetryData[0].engineRPM;
            auto drs = telemetryData->carTelemetryData[0].drs;
            auto revLightsPercent = telemetryData->carTelemetryData[0].revLightsPercent;
            json j;
            j["uid"] = sessionUID;
            j["session_time"] = packet_time;
            j["telemetry"] = {
                { "speed_kph", speed },
                { "throttle_pct", throttle },
                { "steer", steer },
                { "brake_pct", brake }, 
                { "gear", gear },
                { "rpm", engineRPM },
                { "drs", drs }, 
                { "rev_lights_pct", revLightsPercent }
            };
            // std::cout << j.dump(4) << std::endl;
            telFile << j;
        }
        break;
        case 7:
        break;
        default:
        std::cerr << "NO SUCH PACKET";
        break;
        }
    }
}
