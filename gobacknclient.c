#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8000

int i = 0, j = 0;

int main()
{
    struct sockaddr_in serveraddress;

    int clientsocket;
    clientsocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientsocket < 0)
    {
        printf("Socket creation failed\n");
        return -1;
    }

    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_addr.s_addr = INADDR_ANY;

    int connection_status;
    connection_status = connect(clientsocket, (struct sockaddr *)&serveraddress, sizeof(serveraddress));
    if (connection_status < 0)
    {
        printf("Connection failed\n");
        return -1;
    }
    else
        printf("Connection established\n");

     
    int Tt, Tp, N, datasize;
    printf("Enter the transmission time and propagation time in ms: ");
    scanf("%d %d", &Tt, &Tp);    

    send(clientsocket, &Tt, sizeof(Tt), 0);
    send(clientsocket, &Tp, sizeof(Tp), 0);

    printf("Enter the value of N (sender's window): ");
    scanf("%d", &N);
    send(clientsocket, &N, sizeof(N), 0);

    printf("Enter the size of data to be transmitted: ");
    scanf("%d", &datasize);
    send(clientsocket, &datasize, sizeof(datasize), 0);

    int data[datasize];
    printf("Enter the data bits: ");
    for (int i = 0; i < datasize; i++) {
        scanf("%d", &data[i]);
    }

    int lost_packet;
    printf("Enter the packet to be lost while transmission: ");
    scanf("%d", &lost_packet);
    send(clientsocket, &lost_packet, sizeof(lost_packet), 0);

    int i = 0;
    int timer = 0, timer1 = 0;

    while (i < datasize) {
        int x = 0;

        for (int j = i; j < i + N && j < datasize; j++) {
            timer = timer + Tt;
            printf("Data packet %d sent at time %d ms\n", j, timer);

            timer1 = timer + Tp;
            send(clientsocket, &timer1, sizeof(timer1), 0);
        }

        for (int j = i; j < i + N && j < datasize; j++) {
            if (j != lost_packet - 1) {
                int timer;
                recv(clientsocket, &timer, sizeof(&timer),0);
                printf("Acknowledgment for packet %d received at time %d ms\n", j, timer);
                x++;
            } else {
                printf("Acknowledgment for packet %d not received\n", j);
                printf("Retransmitting window\n");

                // Retransmit all unacknowledged packets in the current window
                for (int k = i; k < i + x; k++) {
                    send(clientsocket,&k,sizeof(k),0);
                }

                // Update timer for the lost packet
                int retransmitTimer = (j + 1) * Tp;
                printf("Retransmitted packet %d at time %d ms\n", j, retransmitTimer);

                // Move the window to the next unacknowledged packet
                i = j + 1;

                // Break from the loop since retransmission occurred
                break;
            }
        }
        
        // Move the window to the next set of packets
        i += x;
    }

    // Close the socket
    close(clientsocket);

    return 0;
}

