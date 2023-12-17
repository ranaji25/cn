#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <math.h>

#define PORT 8000

int Tt, Tp, n, packets, Sw, get, sendack = 0;
int timer = 0;
int i, j;

int main()
{
    struct sockaddr_in serveraddress;

    int clientsocket;
    clientsocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientsocket < 0)
    {
        printf("socket creation failed\n");
        return -1;
    }

    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_addr.s_addr = INADDR_ANY;

    int connection_status;
    connection_status = connect(clientsocket, (struct sockaddr *)&serveraddress, sizeof(serveraddress));
    if (connection_status < 0)
    {
        printf("connection failed\n");
        return -1;
    }
    else
        printf("connection established\n");

    printf("Enter the transmission time in milliseconds: ");
    scanf("%d", &Tt);
    send(clientsocket, &Tt, sizeof(Tt), 0);

    printf("Enter the propagation time in milliseconds: ");
    scanf("%d", &Tp);
    send(clientsocket, &Tp, sizeof(Tp), 0);

    int packets = 1 + 2 * (Tp / Tt);
    printf("Maximum number of packets that can be sent: %d\n", packets);
    send(clientsocket, &packets, sizeof(packets), 0);

    int n = floor(log2(packets));
    printf("Number of bits in sequence number(n): %d\n", n);
    send(clientsocket, &n, sizeof(n), 0);

    int Sw = packets - n;
    printf("Number of packets in the sender's window: %d\n", Sw);
    send(clientsocket, &Sw, sizeof(Sw), 0);

    int slidingwindow[Sw];
    int a[Sw]; // Fix 3: Use the correct array size

    int time1 = 0, time2 = 0, k = 0; // Fix 4: Initialize time1 and time2
    int ack; // Fix 5: Declare ack variable
    int recvack = 0;
    while (j < Sw)
    {
        send(clientsocket, &j, sizeof(j), 0);
        send(clientsocket, &time2, sizeof(time2), 0);
        printf("\nPacket no. %d transferred at %d ms", j, time1);
        a[k] = j;
        k++;
        time1 = time1 + Tt;
        time2 = time2 + Tt;
        j++;
    }
    printf("\nSliding window:");
    for (int i = 0; i < Sw; i++)
        printf("%d ", a[i]);
    while(j < packets)
    {
        recv(clientsocket, &ack, sizeof(ack), 0);
        printf("\nAcknowledgement for packet no. %d received at %d ms",recvack,ack);
        for(i=0;i<Sw;i++)
        {
            a[i]=a[i+1];
        }
        a[i-1]=j;
        printf("\nSliding window:");
        for(int i=0;i<Sw;i++)
            printf("%d ",a[i]);
        recvack++;
        time1=ack+Tt;
        time2=time1+Tp+Tt;
        send(clientsocket,&j,sizeof(j),0);
        printf("\nPacket no. %d transferred at %d ms",j,time1);
        send(clientsocket,&time2,sizeof(time2),0);
        j++;
    }
    while (recvack < packets)
    {
        recv(clientsocket, &ack, sizeof(ack), 0);
        printf("\nAcknowledgement for packet no. %d received at %d ms\n", recvack, ack);
        recvack++;
    }
    close(clientsocket);

    return 0;
}
