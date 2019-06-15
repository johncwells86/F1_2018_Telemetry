#include <iostream>
#include <string>
#include <sstream>
#include <chrono>
#include "PacketAnalyzer.h"

void PacketAnalyzer::run()
{
  while (true) {
    // std::this_thread::sleep_for(std::chrono::milliseconds(100));

    NetworkPacket in_packet = q->pop_item();

    struct PacketHeader* strucPtr=&in_packet.header;
    // printf("Sizeof packetheader: %d \n", sizeof(PacketHeader));
    unsigned char* charPtr=(unsigned char*)strucPtr;
    // printf("Size of packet received: (%d)\n", sizeof(in_packet));
    // for(int i=0;i<sizeof(in_packet.header); i++)
    // {
    //     printf("%02x ",charPtr[i]);
    //     // memset()
    // }

    // for(int i=0;i<sizeof(in_packet.data); i++)
    // {
    //     printf("%02x ",in_packet.data[i]);
    //     // memset()
    // }
    // printf("\n");
    uint8_t packet_type = 127;
    packet_type = in_packet.header.packetId;
    // printf("PACKET FORMAT: %d | %X\n", in_packet.header.packetFormat, in_packet.header.packetFormat);
    // printf("PACKET VERSION: %d | %X\n", in_packet.header.packetVersion, in_packet.header.packetVersion);
    // printf("PACKET TYPE: %d | %X \n", in_packet.header.packetId, in_packet.header.packetId);
    // printf("Session UID: %u | %X \n", in_packet.header.sessionUID, in_packet.header.sessionUID);
    // printf("Session time: %F | %X\n", in_packet.header.sessionTime, *(float *)&in_packet.header.sessionTime);
    // printf("FRAME ID   : %d | %X \n", in_packet.header.frameIdentifier, in_packet.header.frameIdentifier);
    // printf("CAR IDX    : %d | %X \n", in_packet.header.playerCarIndex, in_packet.header.playerCarIndex);
    float packet_time = in_packet.header.sessionTime;
    int playerCarIndex = in_packet.header.playerCarIndex;
    PacketMotionData pmd;
    // struct CarMotionData* cmdp=&in_packet.data;
    // printf("Sizeof packetheader: %d \n", sizeof(PacketHeader));
    // unsigned char* cmdPtr=(unsigned char*)strucPtr;
    switch (packet_type) {
    case 0:
        // std::cout << "Motion" << std::endl;
        // for(int i = 0; i < 20; i++)
        //   printf("%02x ", cmdPtr[i]);
        // cmd = CarMotionData(in_packet.data);
        memcpy(&pmd, &in_packet.data, sizeof(pmd));
        printf("motion worldpos X,Y,Z: %f | %f | %f \n", pmd.carMotionData[playerCarIndex].worldPosX, pmd.carMotionData[playerCarIndex].worldPosY, pmd.carMotionData[playerCarIndex].worldPosZ);
        break;
    case 1:
        // std::cout << "Session" << std::endl;
        break;
    case 2:
        // std::cout << "Lap" << std::endl;
        break;
    case 3:
        // std::cout << "Event" << std::endl;
        break;
    case 4:
        // std::cout << "Participants" << std::endl;
        break;
    case 5:
        // std::cout << "Car Setups" << std::endl;
        break;
    case 6:
    {
        // std::cout << "Telemetry" << std::endl;
        // bytes_in = read_packet(sock, packet_data, 1085, &client, &client_len);
        // for(int j = 0; j < sizeof(in_packet.header); j++)  
            // fprintf(stdout, "%02X", &in_packet + j, j % 8 == 0 ? "\n" : "");
        PacketCarTelemetryData* telemetryData = reinterpret_cast<PacketCarTelemetryData*>(&in_packet.data);
        uint8_t player_car_index = 0;
        player_car_index = in_packet.header.playerCarIndex;
        CarTelemetryData* player_car_telemetry = (reinterpret_cast<CarTelemetryData*>(in_packet.data))+player_car_index;
        uint8_t throttle = 0;
        throttle = player_car_telemetry->throttle;
        uint8_t brake = 0;
        brake = player_car_telemetry->brake;
        int8_t steer = -1;
        steer = ((player_car_telemetry->steer / 2) + 50);
        // double timestamp = packet_time;
      
      // std::stringstream ss;
      // ss << "Player Car Idx: (" << player_car_index << ")" << std::endl;
      // ss << "Throttle: " << throttle << std::endl;
      // ss << "Brake: " << brake << std::endl;
      // std::cout << ss.str();
      // printf("Timestamp: %F\n",packet_time);//std::cout << "Trampstamp: " << std::endl;
        }
      // emit packetAnalyzed(QPointF(timestamp, throttle), QPointF(timestamp, brake), QPointF(timestamp, steer));
      //TODO: Emit packet analyzed
    break;
    case 7:
      //bytes_in = read_packet(sock, packet_data, 1061, &client, &client_len);
      //std::cout << "Car Status";
      break;
    default:
      std::cerr << "NO SUCH PACKET";
      break;
    }
  }
}
