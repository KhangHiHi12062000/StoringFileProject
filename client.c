//
// Created by khangnt-leo on 07/07/2022.
//

#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include "tfp.h"
#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 8888
#define BUFF_SIZE 1027

int main(){
    int client_sock;


    struct sockaddr_in server_addr; /* server's address information */
    int msg_len, bytes_sent, bytes_received;

    //Step 1: Construct socket
    client_sock = socket(AF_INET,SOCK_STREAM,0);

    //Step 2: Specify server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);

    //Step 3: Request to connect server
    if(connect(client_sock, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) < 0){
        printf("\nError!Can not connect to sever! Client exit imediately! ");
        return 0;
    }

    //Step 4: Communicate with server

    //send message

    while(1){
        char buff[BUFF_SIZE + 1];
        char *bufptr;

        memset(buff,0,BUFF_SIZE);
        bufptr = buff;
        printf("\nInsert string to send:");
        //fgets(buff, BUFF_SIZE, stdin);
        scanf("%s",buff);
        msg_len = strlen(buff);

        bytes_sent = send(client_sock, buff, msg_len, 0);
        if(bytes_sent < 0)
            perror("\nError: ");

        //receive echo reply
        bytes_received = recv(client_sock, buff, BUFF_SIZE, 0);
        if (bytes_received < 0)
            perror("\nError: ");
        else if (bytes_received == 0)
            //printf("%s",buff);
            printf("Connection closed.\n");

        buff[bytes_received] = '\0';
        printf("Reply from server:");
        for (int i = 0; i < 100; ++i) {
            printf("%c",buff[i]);
        }
        char name[512];
        unsigned short number,numFile;
        numFile = ntohs(*(unsigned short *)(bufptr+1));
        bufptr = eatString(bufptr+3,name);
        printf("\n name %s", name);
        printf("\nSo 1 %d", bufptr[0]);
        printf("\nSo 2 %d", bufptr[1]);
        //bufptr = eatByte(bufptr,&number);
        number = ntohs(*(unsigned short *)bufptr);
        printf("\nnumber %u",number);
        printf("\nnumFile %u",numFile);

    }

    //Step 4: Close socket
    close(client_sock);
    return 0;
}
