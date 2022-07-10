//
// Created by khangnt-leo on 01/07/2022.
//

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

#define LIMIT 10
#define PORT 8080

void printLocalAddrs();
int SocketTCP();
int equals(struct sockaddr_in* addr1, struct sockaddr_in* addr2);

