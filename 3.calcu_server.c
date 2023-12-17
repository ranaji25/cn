#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8000

int calculate(int opd1, int opd2, char opt) {
    switch (opt) {
        case '+':
            return opd1 + opd2;
        case '-':
            return opd1 - opd2;
        case '*':
            return opd1 * opd2;
        case '/':
            if (opd2 != 0) {
                return opd1 / opd2;
            } else {
                return -1; // Error: division by zero
            }
        default:
            return -1; // Error: invalid operator
    }
}

int main() {
    int serversocket, clientsocket, bindstatus;
    char servermessage[256];
    struct sockaddr_in serveraddress, clientaddress;
    int client_address_len = sizeof(clientaddress);

    serversocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serversocket < 0) {
        printf("socket failed\n");
        return -1;
    }

    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(PORT);
    serveraddress.sin_addr.s_addr = INADDR_ANY;

    bindstatus = bind(serversocket, (const struct sockaddr *)&serveraddress, sizeof(serveraddress));
    if (bindstatus < 0) {
        printf("binding failed\n");
        return -1;
    } else {
        printf("binding is successful\n");
    }

    listen(serversocket, 3);
    printf("Waiting for client connection...\n");

    clientsocket = accept(serversocket, (struct sockaddr *)&clientaddress, (socklen_t *)&client_address_len);
    if (clientsocket < 0) {
        printf("connection is rejected by server\n");
        return -1;
    } else {
        printf("connection is accepted\n");
    }

    while (1) {
        char operator[2];
        char operand1[256];
        char operand2[256];

        recv(clientsocket, operator, sizeof(operator), 0); // Read the operator
        recv(clientsocket, operand1, sizeof(operand1), 0); // Read the first operand
        recv(clientsocket, operand2, sizeof(operand2), 0); // Read the second operand

        // Check for termination condition
        if (operator[0] == '-' && operator[1] == '1') {
            printf("Termination condition received. Closing the connection.\n");
            break;
        }

        int opt = operator[0]; // Store the operator as a character
        int opd1 = atoi(operand1);
        int opd2 = atoi(operand2);

        int res_int = calculate(opd1, opd2, opt);
        char res_char[20];
        sprintf(res_char, "%d", res_int);

        send(clientsocket, res_char, strlen(res_char), 0);
        printf("\nResult: %s\n", res_char);
        
    }

    close(serversocket);
    return 0;
}
