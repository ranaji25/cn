#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8000
#define MAX_MESSAGE_SIZE 1024

int main()
{
    int serversocket, clientsocket, bindstatus;
    char servermessage[256];
    struct sockaddr_in serveraddress, clientaddress;
    int client_address_len = sizeof(clientaddress);
    char message[MAX_MESSAGE_SIZE];

    serversocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serversocket < 0)
    {
        printf("socket failed\n");
        return -1;
    }

    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_addr.s_addr = INADDR_ANY;

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

    clientsocket = accept(serversocket, (struct sockaddr *)&clientaddress, (socklen_t *)&client_address_len);
    if (clientsocket < 0)
    {
        printf("connection is rejected by server\n");
        return -1;
    }
    else
    {
        printf("connection is accepted\n");
    }

    while (1) {
        // Receive a message from the client
        memset(message, 0, MAX_MESSAGE_SIZE);
         
        printf("Client: %s", message);
        if (strcmp(message, "bye\n") == 0) {
        printf("Client said goodbye. Exiting...\n");
        close(clientsocket);
        close(serversocket);
        break; }
        
        // Send a response to the client
        printf("Server: ");
        fgets(message, sizeof(message), stdin); // Read a message from the server
        if (strcmp(message, "bye\n") == 0) {
        printf("Client said goodbye. Exiting...\n");
        close(clientsocket);
        close(serversocket);
        break; }
        send(clientsocket, message, strlen(message), 0);

        
        
    }
    close(serversocket);
    
    return 0;
}