#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8069

int i = 0, j = 0;

int main() {
    struct sockaddr_in serveraddress;

    int clientsocket;
    clientsocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientsocket < 0) {
        perror("socket creation failed");
        return -1;
    }

    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_addr.s_addr = INADDR_ANY;

    int connection_status;
    connection_status = connect(clientsocket, (struct sockaddr *)&serveraddress, sizeof(serveraddress));
    if (connection_status < 0) {
        perror("connection failed");
        return -1;
    } else
        printf("connection established\n");

    int Tt, Tp, datasize, timer = 0;
    printf("Enter the Transmission time and propagation time: ");
    scanf("%d %d", &Tt, &Tp);
    send(clientsocket, &Tt, sizeof(Tt), 0);
    send(clientsocket, &Tp, sizeof(Tp), 0);

    printf("Enter the size of data to be transmitted: ");
    scanf("%d", &datasize);
    send(clientsocket, &datasize, sizeof(datasize), 0);

    int data[datasize];
    printf("Enter the data bits: ");
    for (i = 0; i < datasize; i++) {
        scanf("%d", &data[i]);
    }

    int transmission_timer = 2 * Tp;
    int timeout_timer = 3 * transmission_timer;
    send(clientsocket, &timeout_timer, sizeof(timeout_timer), 0);
    int lost_packet;
    printf("Enter the packet to be lost while transmission: ");
    scanf("%d", &lost_packet);
    send(clientsocket, &lost_packet, sizeof(lost_packet), 0);

    for (i = 0; i < datasize; i++) {
        if (i != lost_packet - 1) {
            timer = timer + Tt + Tp;
            send(clientsocket, &data[i], sizeof(data[i]), 0);
            send(clientsocket, &timer, sizeof(timer), 0);

            recv(clientsocket, &timer, sizeof(timer), 0);
            printf("Ack received at %d ms\n", timer);
        } 
        else {
            printf("Packet sent at time %d\n", timer + Tt);
            timer = timer + timeout_timer + Tt;
            printf("Packet retransmited at time %d\n", timer);
            timer = timer + Tp;
            send(clientsocket, &data[i], sizeof(data[i]), 0);
            send(clientsocket, &timer, sizeof(timer), 0);

            recv(clientsocket, &timer, sizeof(timer), 0);
            printf("Ack received at %d ms\n", timer);
        }
    }

    close(clientsocket);

    return 0;
}
