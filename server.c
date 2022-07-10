#include "server.h"

//#include "tfp.h";


int main(int argc, char **argv){
    int sock, connection;
    struct sockaddr_in client;

    sock = SocketTCP();
    int sin_size;
    pid_t pid;
    AddSigAction();

    while(1){
        sin_size=sizeof(struct sockaddr_in);
        if ((connection = accept(sock, (struct sockaddr *)&client, &sin_size))==-1){
            if (errno == EINTR)
                continue;
            else{
                perror("\nError: ");
                return 0;
            }
        }

        pid = fork();

        if(pid<0){
            printf("Fork ERROR!");
            exit(1);
        }
        else if(pid == 0){
            close(sock);
            printf("You got a connection from %s\n", inet_ntoa(client.sin_addr));
            echo(connection);
            exit(0);
        }
        close(connection);
    }
    close(sock);
    return 0;
}

void sigchld_handler(int signo){
    pid_t pid;
    int stat;
    while(waitpid(-1, stat, WNOHANG)>0)
        printf("\nChild %d terminated\n",pid);
}

void AddSigAction(){
    struct sigaction sa;
    sa.sa_handler=sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags=SA_RESTART;

    signal(SIGCHLD,sigchld_handler);
    if(sigaction(SIGCHLD, &sa, NULL)==-1){
        printf("\x1B[33mERROR:\x1B[0m Sigaction error. %s\n", strerror(errno));
        exit(1);
    }
}
void echo(int sock) {
    char buff[BUFF_SIZE];
    int bytes_sent, bytes_received;

    bytes_received = recv(sock ,buff, BUFF_SIZE, 0); //blocking
    if (bytes_received < 0)
        perror("\nError: ");
    else if (bytes_received == 0)
        printf("Connection closed.");

    bytes_sent = send(sock, buff, bytes_received, 0); /* echo to the client */
    if (bytes_sent < 0)
        perror("\nError: ");

    close(sock);
}
