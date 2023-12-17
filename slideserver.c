#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8000

int Tt, Tp, n, packets, Sw, get, sendack = 0;
int timer = 0;
int i, j;

int main()
{
    char servermessage[256];
    struct sockaddr_in serveraddress, clientaddress;
    int client_address_len = sizeof(clientaddress);

    int serversocket;
    serversocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serversocket < 0)
    {
        printf("socket failed");
        return -1;
    }

    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_addr.s_addr = INADDR_ANY;

    int bindstatus;
    bindstatus = bind(serversocket, (const struct sockaddr *)&serveraddress, sizeof(serveraddress));
    if (bindstatus < 0)
    {
        printf("binding failed\n");
        return -1;
    }
    else
    {
        printf("binding is successful\n");
    }

    listen(serversocket, 3);
    printf("Waiting for client connection...\n");

    int clientsocket;
    clientsocket = accept(serversocket, (struct sockaddr *)&clientaddress, (socklen_t *)&client_address_len);
    if (clientsocket < 0)
    {
        printf("connection is rejected by server\n");
        return -1;
    }
    else
        printf("connection is accepted\n");

    recv(clientsocket, &Tt, sizeof(Tt), 0);
    recv(clientsocket, &Tp, sizeof(Tp), 0);
    recv(clientsocket, &packets, sizeof(packets), 0);
    recv(clientsocket, &n, sizeof(n), 0);
    recv(clientsocket, &Sw, sizeof(Sw), 0);

    int count = 0, ack = Tt + Tp + Tt + Tp, timer2;
    while (j < Sw)
    {
        recv(clientsocket, &get, sizeof(get), 0);
        recv(clientsocket, &timer2, sizeof(timer2), 0);
        printf("\nPacket number %d received at %d ms", get, timer2);
        j++;
    }

    while (j < packets)
    {
        send(clientsocket, &ack, sizeof(ack), 0);
        ack = ack + Tt;
        sendack++;
        recv(clientsocket, &get, sizeof(get), 0);
        recv(clientsocket, &timer2, sizeof(timer2), 0);
        printf("\nPacket number %d received at %d ms\n", get, timer2);
        j++;
    }

    while (sendack < packets)
    {
        send(clientsocket, &ack, sizeof(ack), 0);
        sendack++;

        if (sendack == Sw)
            ack = ack + Tp;
        else
            ack = ack + Tt;
    }

    close(serversocket);

    return 0;
}
