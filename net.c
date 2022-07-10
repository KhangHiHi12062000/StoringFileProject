//
// Created by khangnt-leo on 01/07/2022.
//
#include "net.h"


void printLocalAddrs(){
    char hostname[128];
    gethostname(hostname, 128);
    printf(" Host name: %s\n\n", hostname);

    struct ifaddrs *addr, *paddr;
    if(getifaddrs(&addr)==-1){
        printf("\x1B[33m WARNING:\x1B[0m Unknown local address.\n");
    }
    paddr=addr;

    char host[NI_MAXHOST];

    printf(" Interfaces:\nAddress \t Netmask\n");
    for(;addr!=NULL;addr=addr->ifa_next){
        if(addr->ifa_addr->sa_family==AF_INET){
            getnameinfo(addr->ifa_addr, sizeof(struct sockaddr_in), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
            printf("%s\t", host);
            getnameinfo(addr->ifa_netmask, sizeof(struct sockaddr_in), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
            printf("%s\n", host);
        }
    }
    printf("\n");
    freeifaddrs(paddr);
}

int SocketTCP(){
    int sock,status;
    struct sockaddr_in addr;
    sock = socket(PF_INET, SOCK_STREAM, 0);

    if(sock<0){
        printf("\nCreate socket fail!");
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(sock, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
        printf("\nBinding ERROR!");
    }

    status = listen(sock, LIMIT);
    if(status<0){
        printf("\nListening ERROR!");
    }

    printf("Server listening to ...\n");

    return sock;
}

int equals(struct sockaddr_in* addr1, struct sockaddr_in* addr2){
    unsigned short port1=addr1->sin_port;
    unsigned short port2=addr2->sin_port;

    if(port1!=port2) return 0;

    char host1[NI_MAXHOST];
    char host2[NI_MAXHOST];
    inet_ntop(AF_INET, &(addr1->sin_addr), host1, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &(addr2->sin_addr), host2, INET_ADDRSTRLEN);

    if(strcmp(host1, host2)!=0) return 0;

    return 1;
}

