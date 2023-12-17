#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8000
#define N 


int main()
{
    struct sockaddr_in serveraddress;

    int clientsocket = socket(AF_INET, SOCK_STREAM, 0);
    if(clientsocket<0)
    {
        printf("socket creation failed\n");
        return -1;
    }

    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_addr.s_addr = INADDR_ANY;

    int connection_status = connect(clientsocket, (struct sockaddr *)&serveraddress, sizeof(serveraddress));
    if(connection_status < 0)
    {
        printf("connection failed\n");
        return -1;
    }
    else
        printf("connection established\n");

while (1) {
		char operator[2];
		char operand1[256];
		char operand2[256];

		printf("\nEnter Operator (+, -, *, /) or -1 to end: ");
		scanf("%[^\n]%*c", operator);
		operator[strlen(operator)] = '\0';
		send(clientsocket, operator, strlen(operator) + 1, 0);

		printf("Enter Operand 1: ");
		scanf("%[^\n]%*c", operand1);
		operand1[strlen(operand1)] = '\0';
		send(clientsocket, operand1, strlen(operand1) + 1, 0);

		printf("Enter Operand 2: ");
		scanf("%[^\n]%*c", operand2);
		operand2[strlen(operand2)] = '\0';
		send(clientsocket, operand2, strlen(operand2) + 1, 0);

		char res_char[20];
	    recv(clientsocket, res_char, sizeof(res_char), 0);
	    printf("\nResult: %s\n", res_char);
    }

    close(clientsocket);
    return 0;
}

