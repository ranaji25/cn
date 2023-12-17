#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8000

int i = 0, j = 0;

int main() {
    char servermessage[256];
    struct sockaddr_in serveraddress, clientaddress;
    int client_address_len = sizeof(clientaddress);

    int serversocket;
    serversocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serversocket < 0) {
        perror("socket failed");
        return -1;
    }

    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_addr.s_addr = INADDR_ANY;

    int bindstatus;
    bindstatus = bind(serversocket, (const struct sockaddr *)&serveraddress, sizeof(serveraddress));
    if (bindstatus < 0) {
        perror("binding failed");
        return -1;
    } else {
        printf("binding is successful\n");
    }

    listen(serversocket, 3);
    printf("Waiting for client connection...\n");

    int clientsocket;
    clientsocket = accept(serversocket, (struct sockaddr *)&clientaddress, (socklen_t *)&client_address_len);
    if (clientsocket < 0) {
        perror("connection is rejected by server");
        return -1;
    } else
        printf("connection is accepted\n");

    int Tt, Tp, datasize, timer = 0;
    recv(clientsocket, &Tt, sizeof(Tt), 0);
    recv(clientsocket, &Tp, sizeof(Tp), 0);
    recv(clientsocket, &datasize, sizeof(datasize), 0);

    int data[datasize];

    while (i < datasize) {
        recv(clientsocket, &data[i], sizeof(data[i]), 0);
        recv(clientsocket, &timer, sizeof(timer), 0);
        printf("Data packet with data %d received at %d\n", data[i], timer);

        timer = timer + Tt + Tp;
        send(clientsocket, &timer, sizeof(timer), 0);

        i++;
    }

    close(serversocket);

    return 0;
}
