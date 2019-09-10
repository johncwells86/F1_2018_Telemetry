#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <ostream>
#include <iomanip>
#include <chrono>



#include "PacketAnalyzer.h"

using namespace nlohmann;

PacketAnalyzer::PacketAnalyzer(SafeQueue<NetworkPacket>* q): q(q) {
//    clientConfig;
//    dynamoClient(clientConfig);

}

void PacketAnalyzer::run() {
    std::ofstream telFile;
    telFile.open("f1.json", std::ios::out | std::ios::trunc);
    json j = "{ \"dummy\": true}"_json;
    // HACK HACK HACK this is bad and I feel bad.
    telFile << "[" << std::setw(4) << j << std::endl;
    bool go = true;
    while (go) {
        struct NetworkPacket in_packet = q->pop_item();
    
        uint64_t sessionUID = in_packet.header.sessionUID;
        uint8_t packet_type = in_packet.header.packetId;
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

            auto localVelocityX =  motionData->localVelocityX;
            auto localVelocityY =  motionData->localVelocityY;
            auto localVelocityZ =  motionData->localVelocityZ;
            auto angularVelocityX =  motionData->angularVelocityX;
            auto angularVelocityY =  motionData->angularVelocityY;
            auto angularVelocityZ =  motionData->angularVelocityZ;
            auto angularAccelerationX = motionData->angularAccelerationX;
            auto angularAccelerationY = motionData->angularAccelerationY;
            auto angularAccelerationZ = motionData->angularAccelerationZ;
            auto frontWheelsAngle =  motionData->frontWheelsAngle;

            json j;
            j["world_pos_X"] = worldPosX;
            j["world_pos_y"] = worldPosY;
            j["world_pos_z"] = worldPosZ;
            j["velocity_x"] =  worldVelX;
            j["velocity_y"] =  worldVelY;
            j["velocity_z"] = worldVelZ;
            j["direction_fwd"] = {worldForwardDirX, worldForwardDirY, worldForwardDirZ};
            j["direction_right"] = {worldRightDirX, worldRightDirY, worldRightDirZ};


            j["suspension_pos"] = { { motionData->suspensionPosition[0], motionData->suspensionPosition[1], motionData->suspensionPosition[2], motionData->suspensionPosition[3] } };
            j["suspension_vel"] = { { motionData->suspensionVelocity[0], motionData->suspensionVelocity[1], motionData->suspensionVelocity[2], motionData->suspensionVelocity[3] } };
            j["suspension_acc"] = { { motionData->suspensionAcceleration[0], motionData->suspensionAcceleration[1], motionData->suspensionAcceleration[2], motionData->suspensionAcceleration[3] } };
            j["wheels_speed"] = { { motionData->wheelSpeed[0], motionData->wheelSpeed[1], motionData->wheelSpeed[2], motionData->wheelSpeed[3] } };
            j["wheels_slip"] = { { motionData->wheelSlip[0], motionData->wheelSlip[1], motionData->wheelSlip[2], motionData->wheelSlip[3] } };
            j["local_vel_x"] =        localVelocityX;
            j["local_vel_y"] =        localVelocityY;
            j["local_vel_z"] =        localVelocityZ;
            j["angular_vel_x"] =      angularVelocityX;
            j["angular_vel_y"] =      angularVelocityY;
            j["angular_vel_z"] =      angularVelocityZ;
            j["angular_accel_x"] =    angularAccelerationX;
            j["angular_accel_y"] =    angularAccelerationY;
            j["angular_accel_z"] =    angularAccelerationZ;
            j["front_wheels_angle"] = frontWheelsAngle;
            j["session_time"] = packet_time;
            j["session_uid"] = sessionUID;
            j["table"] = "motion_data";

            telFile << ","  << std::setw(4) << j << std::endl;
            break;
        }
        case 1:
        {
            struct PacketSessionData *sessData = (struct PacketSessionData *)&in_packet.data;

            auto weather = sessData->weather;                     // Weather - 0 = clear, 1 = light cloud, 2 = overcast
                                                                  // 3 = light rain, 4 = heavy rain, 5 = storm
            auto trackTemperature = sessData->trackTemperature;   // Track temp. in degrees celsius
            auto airTemperature = sessData->airTemperature;       // Air temp. in degrees celsius
            auto totalLaps = sessData->totalLaps;                 // Total number of laps in this race
            auto trackLength = sessData->trackLength;             // Track length in metres
            json j;

            j["weather"] = weather;
            j["air_temp_c"] = airTemperature;
            j["track_temp_c"] = trackTemperature;
            j["total_laps"] = totalLaps;
            j["track_length_meters"] = trackLength;
            j["session_uid"] = sessionUID;
            j["session_time"] = packet_time;
            j["type"] = "session_data";
            telFile << ","  << std::setw(4) << j << std::endl;
            break;
        }
        case 2:
        {
            struct PacketLapData *lapData = (struct PacketLapData *)&in_packet.data;
            json j;
            j["session_time"] = packet_time;
            j["session_uid"] = sessionUID;
            j["table"] = "lap_data";
            for (auto i = 0; i < 20; i++) {
                LapData l = lapData->lapData[i];
                auto lastLapTime = l.lastLapTime;
                auto currentLapTime = l.currentLapTime;
                auto bestLapTime = l.bestLapTime;
                auto sector1Time = l.sector1Time;
                auto sector2Time = l.sector2Time;
                auto lapDistance = l.lapDistance;
                auto totalDistance = l.totalDistance;
                auto safetyCarDelta = l.safetyCarDelta;
                auto carPosition = l.carPosition;
                auto currentLapNum = l.currentLapNum;
                auto pitStatus = l.pitStatus;
                auto sector = l.sector;
                auto currentLapInvalid = l.currentLapInvalid;
                auto penalties = l.penalties;
                auto gridPosition = l.gridPosition;
                auto driverStatus = l.driverStatus;
                auto resultStatus = l.resultStatus;
                j["lastLapTime"] =        lastLapTime;
                j["currentLapTime"] =     currentLapTime;
                j["bestLapTime"] =        bestLapTime;
                j["sector1Time"] =        sector1Time;
                j["sector2Time"] =        sector2Time;
                j["lapDistance"] =        lapDistance;
                j["totalDistance"] =      totalDistance;
                j["safetyCarDelta"] =     safetyCarDelta;
                j["carPosition"] =        carPosition;
                j["currentLapNum"] =      currentLapNum;
                j["pitStatus"] =          pitStatus;
                j["sector"] =             sector;
                j["currentLapInvalid"] =  currentLapInvalid;
                j["penalties"] =          penalties;
                j["gridPosition"] =       gridPosition;
                j["driverStatus"] =       driverStatus;
                j["resultStatus"] =       resultStatus;
            }
            telFile << ","  << std::setw(4) << j << std::endl;
            break;
        }
        case 3:
        {
            struct PacketEventData *eventData = (struct PacketEventData *)&in_packet.data;
            json j;
            j["session_time"] = packet_time;
            j["session_uid"] = sessionUID;
            j["table"] = "event_data";
            break;
        }
        case 4:
        {
            struct PacketParticipantsData *partData = (struct PacketParticipantsData *)&in_packet.data;
            json j;
            j["session_time"] = packet_time;
            j["session_uid"] = sessionUID;
            j["table"] = "participants_data";
            break;
        }
        case 5:
        {
            struct PacketCarSetupData *setupData = (struct PacketCarSetupData *)&in_packet.data;
            json j;
            j["session_time"] = packet_time;
            j["session_uid"] = sessionUID;
            j["table"] = "car_setup";
            for (auto i = 0; i < 20; i++) {
                CarSetupData c = setupData->carSetups[i];
                auto frontWing             = c.frontWing;
                auto rearWing              = c.rearWing;
                auto onThrottle            = c.onThrottle;
                auto offThrottle           = c.offThrottle;
                auto frontCamber           = c.frontCamber;
                auto rearCamber            = c.rearCamber;
                auto frontToe              = c.frontToe;
                auto rearToe               = c.rearToe;
                auto frontSuspension       = c.frontSuspension;
                auto rearSuspension        = c.rearSuspension;
                auto frontAntiRollBar      = c.frontAntiRollBar;
                auto rearAntiRollBar       = c.rearAntiRollBar;
                auto frontSuspensionHeight = c.frontSuspensionHeight;
                auto rearSuspensionHeight  = c.rearSuspensionHeight;
                auto brakePressure         = c.brakePressure;
                auto brakeBias             = c.brakeBias;
                auto frontTyrePressure     = c.frontTyrePressure;
                auto rearTyrePressure      = c.rearTyrePressure;
                auto ballast               = c.ballast;
                auto fuelLoad              = c.fuelLoad;
                j["aero"]["front_wing"] =                  frontWing;
                j["aero"]["rear_wing"] =                   rearWing;
                j["differential"]["on_throttle"] =         onThrottle;
                j["differential"]["off_throttle"] =        offThrottle;
                j["camber"]["front_camber"] =              frontCamber;
                j["camber"]["rear_camber"] =               rearCamber;
                j["camber"]["front_toe"] =                 frontToe;
                j["camber"]["rear_toe"] =                  rearToe;
                j["suspension"]["front_suspension"] =      frontSuspension;
                j["suspension"]["rear_suspension"] =       rearSuspension;
                j["suspension"]["front_anti_roll_bar"] =   frontAntiRollBar;
                j["suspension"]["rear_anti_roll_bar"] =    rearAntiRollBar;
                j["suspension"]["front_height"] =          frontSuspensionHeight;
                j["suspension"]["rear_height"] =           rearSuspensionHeight;
                j["brakes"]["brake_pressure"] =            brakePressure;
                j["brakes"]["brake_bias"] =                brakeBias;
                j["tyres"]["front_tyre_pressure"] =        frontTyrePressure;
                j["tyres"]["rear_tyre_pressure"] =         rearTyrePressure;
                j["ballast"] =                             ballast;
                j["fuel"]["fuel_kg"] =                     fuelLoad;
            }
            telFile << ","  << std::setw(4) << j << std::endl;
            break;
        }
        case 6: {
            struct PacketCarTelemetryData *telemetryData = (struct PacketCarTelemetryData *) &in_packet.data;
            json j;
            j["session_uid"] = sessionUID;
            j["session_time"] = packet_time;
            j["table"] = "telemetry_data";

            auto speed = telemetryData->carTelemetryData[playerCarIndex].speed;
            auto throttle = telemetryData->carTelemetryData[playerCarIndex].throttle;
            auto steer = telemetryData->carTelemetryData[playerCarIndex].steer;
            auto brake = telemetryData->carTelemetryData[playerCarIndex].brake;
            auto gear = telemetryData->carTelemetryData[playerCarIndex].gear;
            auto engineRPM = telemetryData->carTelemetryData[playerCarIndex].engineRPM;
            auto drs = telemetryData->carTelemetryData[playerCarIndex].drs;
            auto revLightsPercent = telemetryData->carTelemetryData[playerCarIndex].revLightsPercent;
            j["speed_kph"] = speed;
            j["throttle_pct"] = throttle;
            j["steer"] = steer;
            j["brake_pct"] = brake;
            j["gear"] = gear;
            j["rpm"] = engineRPM;
            j["drs"] = drs;
            j["rev_lights_pct"] = revLightsPercent;

            telFile << "," << std::setw(4) << j << std::endl;
            break;
        }
        case 7:
            break;
        default:
            std::cerr << "NO SUCH PACKET";
        break;
        }
        telFile.flush();
    }
}

void PacketAnalyzer::pushToDynamoDB(json j, const std::string & table) {
//
    Aws::DynamoDB::Model::PutItemRequest pir;
    pir.SetTableName(table);

    Aws::DynamoDB::Model::AttributeValue av;
    av.SetS(name);
//    pir.AddItem("Name", av);
//
//    for (int x = 3; x<argc; x++) {
//        const Aws::String arg(argv[x]);
//        const Aws::Vector <Aws::String> &flds = Aws::Utils::StringUtils::Split(arg, '=');
//        if (flds.size() == 2) {
//            Aws::DynamoDB::Model::AttributeValue val;
//            val.SetS(flds[1]);
//            pir.AddItem(flds[0], val);
//        }
//    }
//
//    const Aws::DynamoDB::Model::PutItemOutcome result = dynamoClient.PutItem(pir);
//    if (!result.IsSuccess()) {
//        std::cout << result.GetError().GetMessage() << std::endl;
//        return 1;
//    }
}
