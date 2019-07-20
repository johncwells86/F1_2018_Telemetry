#include "UDPListener.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <iostream>
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>
#include "packets.h"

UDPListener::UDPListener(int port, SafeQueue<NetworkPacket>* q)
  : port(port), q(q) {

    // run();
}

UDPListener::~UDPListener() {

}

void UDPListener::run() {

    int sockfd = -1; 
    char buffer[2048]; 
    struct sockaddr_in servaddr, cliaddr; 
      
    sockfd = ::socket(AF_INET, SOCK_DGRAM, 0);
    // Creating socket file descriptor 
    if(sockfd < 0) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
      
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 
      
    // Filling server information 
    servaddr.sin_family        = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr   = INADDR_ANY; 
    servaddr.sin_port          = htons(port); 
      
    // Bind the socket with the server address 
    if(bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
      
    uint len;
    int n; 
    printf("Listening for traffic on port: %d\n", port);
    bool go = true;
    while(go) {
        NetworkPacket data;
        n = recvfrom(sockfd, (char *)&buffer, sizeof(buffer), 0, ( struct sockaddr *) &cliaddr, &len);
        if(n < 0) {
            memset(&buffer, 0, sizeof(buffer));
            printf("ERRORRRORRRRORRRRR!");
            continue;
        } 

        memcpy(&data.header.packetFormat, buffer, sizeof(uint16_t));
        memcpy(&data.header.packetVersion, buffer + 2, sizeof(uint8_t));
        memcpy(&data.header.packetId, buffer + 3, sizeof(uint8_t)); 
        memcpy(&data.header.sessionUID, buffer + 4, sizeof(uint64_t));
        memcpy(&data.header.sessionTime, buffer + 12, sizeof(float)); 
        memcpy(&data.header.frameIdentifier, buffer + 16, sizeof(uint32_t));
        memcpy(&data.header.playerCarIndex, buffer + 20, sizeof(uint8_t));
        
        memcpy(&data.data, buffer, sizeof(buffer));

        // for(int i=21;i<n; i++)
        // {
        //     printf("%02x ",buffer[i]);
        // // memset()
        // }
        // printf("\n---------------------------\n");
  
        q->push_item(data);

        memset(&buffer, 0, sizeof(buffer));
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}
