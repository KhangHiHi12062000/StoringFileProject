//
// Created by khangnt-leo on 28/06/2022.
//
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
#include "file.h"
#include "net.h"
#include "tfp.h"

#define BUFF_SIZE 1024
#define SERVER_ROOT ""

void sigchld_handler();
void AddSigAction();
void echo(int sock);