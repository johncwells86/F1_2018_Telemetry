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
    int i = 0;
    while(go) {
        NetworkPacket data;
        n = recvfrom(sockfd, (char *)buffer, sizeof(buffer), 0, ( struct sockaddr *) &cliaddr, &len);
        if(n < 0) {
            memset(&buffer, 0, sizeof(data.data));
            printf("ERRORRRORRRRORRRRR!");
            continue;
        } 
        if (i % 1000 == 0) { 

            std::cout << "Are you receiving me?" << std::endl;
        }
        // printf("Received %d bytes from UDP Thingy.\n", n);
        memcpy(&data.header, buffer, sizeof(data.header));
        // memcpy(&data.data, buffer + sizeof(data.header), sizeof(buffer) - sizeof(data.header));
        // data.data[n] = '\0'; 
        // write(1,data.data,n);
        q->push_item(data);
        i++;
        // for(int i = 0; i < n; ++i)
            // fprintf(stdout, "%02X%s", buffer[i], ( i + 1 ) % 16 == 0 ? "\n" : " " );
        // printf("Client : %s\n", buffer);
        memset(&buffer, 0, sizeof(buffer));

    }

    // sendto(sockfd, (const char *)hello, strlen(hello),  
    //     MSG_CONFIRM, (const struct sockaddr *) &cliaddr, 
    //         len); 
    // printf("Hello message sent.\n");  
}
