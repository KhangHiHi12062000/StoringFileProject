#include "server.h"

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
void  socketThread(int  clientSocket)
{   char cdirectory[255];
    char client_message[BUFF_SIZE];

    char *cliptr, *bufptr;
    int buff_size;
    int newSocket = clientSocket;
    int loop = 1;
    while(loop) {
        if(recv(newSocket, client_message, BUFF_SIZE, 0)>0){
//
            printf("\n%s\n", client_message);

            //reset 1 buffer
            char buffer[BUFF_SIZE];
            //printf("%s",client_message);
            cliptr = client_message;
            bufptr = buffer;
            buff_size = 0;
            memset(buffer, '\0', BUFF_SIZE);
            //Send message to the client socket
            pthread_mutex_lock(&lock);
            char *opcode = malloc(sizeof(char) * 10);
            char *username = malloc(sizeof(char) * BUFF_SIZE);
            char *filename = malloc(BUFF_SIZE * sizeof(char));
            memset(username, 0, BUFF_SIZE);
            memset(filename, 0, BUFF_SIZE);
            *opcode = client_message[0];
            switch (*opcode) {
                case CREATE_FOLDER: {
                    cliptr = eatString(cliptr + 1, username);
                    strcpy(cdirectory, SERVER_ROOT);
                    strcat(cdirectory, "/");
                    strcat(cdirectory, username);
                    sleep(0.2);
                    create_Folder(cdirectory);
                    //buffer="ok";
                    strcpy(buffer, "ok!");
                    buff_size+=4;
                    send(newSocket, buffer, buff_size, 0);
                    break;
                }
                case LIST_DIR: {
                    //cliptr = eatString(cliptr + 1, username);
//                    strcpy(cdirectory, SERVER_ROOT);
//                    strcat(cdirectory, "/");
//                    strcat(cdirectory, username);
                    unsigned short numFile = 0;
                    char folder_size[16];
                    bufptr[0] = '2';
                    buff_size += 1;
                    File *list = listFiles(cdirectory,&numFile);
                    File *flist = list;
                    //char *data = (char *) malloc(BUFF_SIZE * sizeof(char));
                    //data[0] = 0;
                    *((unsigned short *) folder_size) = htons(numFile);
                    *(bufptr+buff_size) = folder_size[0];
                    *(bufptr+buff_size+1) = folder_size[1];

                    buff_size += 2;
                    while (flist != NULL) {
                        int nlen = strlen(flist->name);
                        int slen;
                        int size;
                        int len = 0;
                        char file_size[16];
                        if (flist->isDir) {
                            strcpy(file_size, "DIR");
                            slen = strlen(file_size);
                        } else {
                            //                    sizeToH(flist->size, file_size, 16);
                            //                    slen=strlen(file_size);
                            *((unsigned short *) file_size) = htons(flist->size);
                            slen = strlen(file_size);
                            //printf("\n %d %d %s %u \n", file_size[0], file_size[1], file_size, flist->size);
                        }
//                        if (size <= (nlen + slen + 1)) {
//                            size = size + 512;
//                            //data = (char *) realloc(data, size * sizeof(char));
//                        }
                        copy(bufptr + buff_size, flist->name, &len);
                        buff_size = buff_size + len;
                        *(bufptr + buff_size) = file_size[0];
                        *(bufptr + buff_size + 1) = file_size[1];
                        buff_size += 2;
                        //printf("%s\n", flist->name);
                        flist = flist->next;
                    }

                    //strcpy(buffer, data);
                    //copyfull(buffer, buffer, buff_size);
                    for (int i = 0; i < 100; ++i) {
                        printf("%c", buffer[i]);
                    }
                    printf("\n%d\n", buff_size);
                    //free(data);
                    sleep(0.2);
                    send(newSocket, buffer, buff_size, 0);
                    bufptr = buffer;
                    numFile = ntohs(*(unsigned short *)(bufptr+1));
                    printf("folder size %u\n",numFile);
                    break;
                }
                case RES_LIST_DIR:

                    break;
                case UPLOAD: {

                    unsigned short numBlock;
                    cliptr = eatString(cliptr + 1, filename);
                    //cliptr = eatByte(cliptr,&numBlock);
                    numBlock = ntohs(*(unsigned short *) cliptr);
                    cliptr += 2;
                    upload_mess(newSocket, buffer, filename, cdirectory, numBlock);
                    break;
                }
                case DOWNLOAD:

                    break;
                case DATA:
                    break;
                case CONFIRM:
                    //numBlock = ntohs((unsigned short *)client_message+1);
                    break;
                case DELETE:

                    break;
                case ERROR:
                    break;
                case EXIT:
                    loop = 0;
                    break;
                default:
                    printf("\nopcode is not exist!!");
                    break;
            }
//            for (int i = 0; i < 100 ;++i) {
//                printf("%c", buffer[i]);
//            }
            free(opcode);
            free(username);
            //free(cdirectory);
            free(filename);

            pthread_mutex_unlock(&lock);
            sleep(0.2);

        }
    }
    sleep(0.2);
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
            //socketThread(newSocket);
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