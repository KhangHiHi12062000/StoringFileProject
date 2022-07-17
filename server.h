//
// Created by khangnt-leo on 28/06/2022.
//
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/wait.h>
#include <errno.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <ifaddrs.h>
#include <errno.h>
#include "file.h"
#include "tfp.h"

#define SERVER_ROOT "/home/khangnt-leo/CLionProjects/ServerProject/Database"
#define BUFF_SIZE 1+2+DATA_SIZE
#define DATA_SIZE 1024
#define LIMIT 50
#define PORT 8080


void sigchld_handler();
void AddSigAction();