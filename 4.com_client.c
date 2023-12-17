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
    struct sockaddr_in serveraddress;
    char message[MAX_MESSAGE_SIZE];
    
    int serversocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serversocket < 0)
    {
        printf("socket failed\n");
        return -1;
    }

    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_addr.s_addr = INADDR_ANY;

    int clientsocket;
    clientsocket = socket(AF_INET, SOCK_STREAM, 0);
    if(clientsocket<0)
    {
        printf("socket creation failed\n");
        return -1;
    }

    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_addr.s_addr = INADDR_ANY;

    int connection_status;
    connection_status = connect(clientsocket, (struct sockaddr *)&serveraddress, sizeof(serveraddress));
    if(connection_status < 0)
    {
        printf("connection failed\n");
        return -1;
    }
    else
        printf("connection established\n");

while (1) {
        // Send a message to the server
        printf("Client: ");
        fgets(message, sizeof(message), stdin); // Read a message from the client
        if (strcmp(message, "bye\n") == 0) {
        printf("Server said goodbye. Exiting...\n");
        close(clientsocket);
        close(serversocket);
        break; }
        send(clientsocket, message, strlen(message), 0);

        // Receive a response from the server
        memset(message, 0, MAX_MESSAGE_SIZE);//used to set a block of memory to data(name,valueineachcell,size)
        
        printf("Server: %s", message);

        if (strcmp(message, "bye\n") == 0) {
        printf("Server said goodbye. Exiting...\n");
        close(clientsocket);
        close(serversocket);
        break; }

    }
    
    close(clientsocket);
    

    return 0;
}

