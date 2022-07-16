#include "server.h"

//void sigchld_handler(int sig){
//    pid_t pid;
//    int status;
//    while(waitpid(-1, &status, WNOHANG)>0)
//        printf("\nChild %d terminated\n",pid);
//}
//
//void AddSigAction(){
//    struct sigaction sa;
//    sa.sa_handler=sigchld_handler;
//    sigemptyset(&sa.sa_mask);
//    sa.sa_flags=SA_RESTART;
//
//    if(sigaction(SIGCHLD, &sa, NULL)==-1){
//        printf("\x1B[33mERROR:\x1B[0m Sigaction error. %s\n", strerror(errno));
//        exit(1);
//    }
//}
//
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
void  socketThread(int  clientSocket)
{
char client_message[BUFF_SIZE];
char *cliptr,*bufptr,*tmpptr;
cliptr = client_message;
char buffer[BUFF_SIZE];
int buff_size;
char tmp[BUFF_SIZE];
tmpptr = tmp;
bufptr = buffer;
//char *opcode;
char cdirectory[255];
//char username[255];
unsigned short numBlock;
int newSocket = clientSocket;
recv(newSocket , client_message , BUFF_SIZE , 0);

//reset 1 buffer
    memset(buffer,0,BUFF_SIZE);
    buff_size=0;
 //Send message to the client socket
pthread_mutex_lock(&lock);
    char *opcode = malloc(sizeof(char)*10);
    char *username = malloc(sizeof(char)*BUFF_SIZE);
    //char *cdirectory = malloc(sizeof(char)*BUFF_SIZE);
    *opcode = client_message[0];
    switch (*opcode) {
        case CREATE_FOLDER:
            cliptr = eatString(cliptr+1, username);
            strcpy(cdirectory,SERVER_ROOT);
            strcat(cdirectory,"/");
            strcat(cdirectory,username);
            strcpy(buffer,"Create folder success!!");
            break;
        case LIST_DIR:
            cliptr = eatString(cliptr+1, username);
            strcpy(cdirectory,SERVER_ROOT);
            strcat(cdirectory,"/");
            strcat(cdirectory,username);
            buffer[0] = '2';
            buff_size += 1;
            File *list=listFiles(cdirectory);
            File *flist=list;
            char *data = (char*)malloc(BUFF_SIZE*sizeof(char));
            data[0] = 0;

            while(flist!=NULL){
                int nlen=strlen(flist->name);
                int slen;
                int size;
                int len=0;
                char file_size[16];
                if(flist->isDir){
                    strcpy(file_size, "DIR");
                    slen=strlen(file_size);
                }else{
//                    sizeToH(flist->size, file_size, 16);
//                    slen=strlen(file_size);
                    *((short *)file_size) = htons(flist->size);
                    slen = strlen(file_size);
                }
                if(size<=(nlen+slen+1)){
                    size=size+512;
                    data=(char*)realloc(data, size*sizeof(char));
                }
                //tmpptr = strcat(tmpptr, flist->name);
                //copy(bufptr+buff_size,flist->name,&len);
                //buff_size = buff_size + len;
                //strcpy(bufptr+buff_size,file_size);
                //buff_size += 2;
                printf("%s\n",flist->name);
                flist = flist->next;
            }
            //strcpy(buffer, data);

            free(data);
            break;
        case RES_LIST_DIR:
            break;
        case UPLOAD:
            //buff = eatString(buff+1, filename);
            break;
        case DOWNLOAD:
            break;
        case DATA:
            break;
        case CONFIRM:
            //buff = eatByte(buff+1, numBlock);
            break;
        case DELETE:
            //buff = eatString(buff+1, filename);
            break;
        case ERROR:
            break;
        case EXIT:
            break;
        default:
            printf("\nopcode is not exist!!");
            break;
    }
    free(opcode);
    free(username);
    //free(cdirectory);

pthread_mutex_unlock(&lock);
sleep(5);
    switch (client_message[0]) {
        case CREATE_FOLDER:
            create_Folder(cdirectory);
            break;
        default:
            break;
    };
    printf("%d\n",buff_size);
    for (int i = 0; i < buff_size; ++i) {
        printf("%c", buffer[i]);
    }
send(newSocket,buffer,BUFF_SIZE,0);
printf("Exit socketThread \n");
close(newSocket);
}

int main(){
    int serverSocket, newSocket;
    struct sockaddr_in serverAddr;
    struct sockaddr_storage serverStorage;
    socklen_t addr_size;
    pid_t pid[50];
    //Create the socket.
    serverSocket = socket(PF_INET, SOCK_STREAM, 0);
    // Configure settings of the server address struct
    // Address family = Internet
    serverAddr.sin_family = AF_INET;
    //Set port number, using htons function to use proper byte order
    serverAddr.sin_port = htons(PORT);

    //Set IP address to localhost
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    //Set all bits of the padding field to 0
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
    //Bind the address struct to the socket
    bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    //Listen on the socket, with 40 max connection requests queued
    if(listen(serverSocket,LIMIT)==0)
    printf("Server listening ....\n");
    else
    printf("Listening Error\n");
    pthread_t tid[60];
    int i = 0;
    while(1)
    {
        /*---- Accept call creates a new socket for the incoming connection ----*/
        addr_size = sizeof serverStorage;
        newSocket = accept(serverSocket, (struct sockaddr *) &serverStorage, &addr_size);
        int pid_c = 0;
        if ((pid_c = fork())==0)
        {
            socketThread(newSocket);
        }
        else
        {
            pid[i++] = pid_c;
            if( i >= 49)
            {
                i = 0;
                while(i < 50){
                    waitpid(pid[i++], NULL, 0);
                    printf("\nChild %d terminated\n",pid[i]);
                }

                i = 0;
            }
        }
    }
    return 0;
}