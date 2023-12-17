#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8000
#define N 100

int i = 0, j = 0;
int stack[N];
int top;


int pop()
{
    if(top == -1)
        return -1;
    else
        return stack[top--];
}

int main()
{
    char servermessage[256];
    struct sockaddr_in serveraddress, clientaddress;
    int client_address_len = sizeof(clientaddress);

    int serversocket;
    serversocket = socket(AF_INET, SOCK_STREAM, 0); 
    if(serversocket < 0)
    {
        printf("socket failed");
        return -1;
    }

    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_addr.s_addr = INADDR_ANY;

    int bindstatus;
    bindstatus = bind(serversocket, (const struct sockaddr*)&serveraddress, sizeof(serveraddress));
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
    if(clientsocket<0)
    {
        printf("connection is rejected by server\n");
        return -1;
    } 
    else
        printf("connection is accepted\n");

    int numofframes;                             // number of frames
    recv(clientsocket, &numofframes, sizeof(numofframes), 0); // recieve

    int framesize[numofframes];                  // array of 3
    for(i=0; i < numofframes; i++)
        recv(clientsocket, &framesize[i], sizeof(framesize[i]), 0);  // receive elems from each frame

    recv(clientsocket, &top, sizeof(top), 0);               // receive top
    for(i = 0; i <= top; i++)
        recv(clientsocket, &stack[i], sizeof(stack[i]), 0);

    int sum = 0;
    for(i = 0; i < numofframes; i++)
        sum = sum + framesize[i];
    
    int count = 0;
    while(top != -1)
    {
        pop(stack[top]);
        count++;
    }

    printf("count: %d\n", count);
    printf("sum: %d\n", sum);

    if(sum == count)
        printf("NO error occured\n");
    else
        printf("Error occured\n");
    

    close(serversocket);

    return 0;
}