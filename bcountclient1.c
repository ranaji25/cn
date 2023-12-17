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
int top = -1;

void push(int k)
{
    stack[++top] = k;
}


int main()
{
    struct sockaddr_in serveraddress;

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

    int k;
    int numofframes;                                                               
    printf("Enter the number of frames you want to transmit: ");   //total number of frames
    scanf("%d", &numofframes);
    send(clientsocket, &numofframes, sizeof(numofframes), 0);      //send frames

    int framesize[numofframes];              // array of size of number of frames
    for(i = 0; i < numofframes; i++)
    {
        printf("Enter the frame size of %dth frame: ", i);  // size of frame 1
        scanf("%d", &framesize[i]);                // scan size
        int x = framesize[i];                      // store size in x
        push(x);                                   // push size in stack
        printf("Enter the data of frame %d: ", i); // data in the frame
        for(j = 1; j < x; j++)                     // loop for data in frame
        { 
            scanf("%d", &k);                       // scan data
            push(k);                               // push in stack
        }
    }

    for(i=0; i < numofframes; i++)
        send(clientsocket, &framesize[i], sizeof(framesize[i]), 0); // send all frames

    printf("value of top: %d", top);
    send(clientsocket, &top, sizeof(top), 0);
    for(i = 0; i <= top; i++)
        send(clientsocket, &stack[i], sizeof(stack[i]), 0);
    

    close(clientsocket);

    return 0;
}