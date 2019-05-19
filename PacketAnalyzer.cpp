#include <iostream>
#include <string>
#include <sstream>
#include "PacketAnalyzer.h"

void PacketAnalyzer::run()
{
  while (true) {
    NetworkPacket in_packet = q->pop_item();
    uint8_t packet_type = in_packet.header.m_packetId;
    float packet_time = in_packet.header.m_sessionTime;
    switch (packet_type) {
    case 0:
      // std::cout << "Motion" << std::endl;
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
      std::cout << "Telemetry" << std::endl;
      // bytes_in = read_packet(sock, packet_data, 1085, &client, &client_len);
        for(int j = 0; j < sizeof(in_packet.header); j++)  
            fprintf(stdout, "%02X", &in_packet + j, j % 8 == 0 ? "\n" : "");
      PacketCarTelemetryData* telemetryData = reinterpret_cast<PacketCarTelemetryData*>(&in_packet);
      uint8_t player_car_index = in_packet.header.m_playerCarIndex;
      CarTelemetryData* player_car_telemetry = (reinterpret_cast<CarTelemetryData*>(in_packet.data))+player_car_index;
      uint8_t throttle = player_car_telemetry->m_throttle;
      uint8_t brake = player_car_telemetry->m_brake;
      int8_t steer = ((player_car_telemetry->m_steer / 2) + 50);
      double timestamp = static_cast<double>(packet_time);

      std::stringstream ss;
      ss << "Player Car Idx: (" << player_car_index << ")" << std::endl;
      ss << "Throttle: " << throttle << std::endl;
      ss << "Brake: " << brake << std::endl;
      ss << "Trampstamp: " << packet_time << std::endl;
      std::cout << ss.str();
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
