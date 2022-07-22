#include "tfp.h"

char *eatString(char *buff, char *str){
    int i = 0;
    while(buff[i] != '\0'){
        str[i] = buff[i];
        i++;
    }
    str[i] = '\0';
    return buff+i+1;
}

char *eatByte(char *buff, unsigned short *numBlock){
    int hi;
    int lo;

    hi = buff[0];
    lo = buff[1];
    *numBlock = ((hi<<8)|lo);
    //*numBlock = hi * 2^8 + lo;
    return buff+2;
}

char *writeString(char *buff, char *str){
    strcpy(buff, str);
    buff[strlen(str)]='\0';
    return buff + strlen(str) + 1;
}

char *writeBytes(char *buff, unsigned short *numBlock){
    unsigned short hi;
    unsigned short lo;

    lo = *numBlock & 0xff;
    hi = (*numBlock>>8) & 0xff;

    buff[0] = hi;
    buff[1] = lo;

    printf("%s",buff);

    return buff + 2;

}

int copy(char *dest, char *src, int *size){
    int i=0;
    while(src[i] != '\0'){
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    *size = i+1;
    return 0;
}

void create_Folder(char *path){
    if(!dirExist(path)){
        createNewFolder(path);
    }
    else{
        printf("Folder %s already exist\n",path);
    }
}

int comfirmMess(int sock, char *buff, int size, unsigned short numblock){
    memset(buff,0,BUFF_SIZE);
    buff[0] = '6';
    send(sock,buff, 1,0);
    return 0;
}
int upload_mess(int sock, char *buff, char *filename,char *cdirectory, unsigned short numblock){
    char tmp[BUFF_SIZE];
    strcpy(tmp, cdirectory);
    strcat(tmp, "/");
    strcat(tmp, filename);

    FILE *file;
//    file = fopen(tmp, "r");
//
//    if(file!=NULL){
//        return 1;
//    }
//    fclose(file);
    file=fopen(tmp, "w+b");
    if(file==NULL){
        return 1;
    }

    int n;
    int ok=0;
    unsigned short packNum=0;

    comfirmMess(sock, buff, 0, packNum);
    int next = 0;
    if(numblock>0) next=1;
    while (next){
        memset(buff,0,BUFF_SIZE);
        n = recv(sock, buff, BUFF_SIZE, 0);
//        for (int i = 0; i < BUFF_SIZE; ++i) {
//            printf("%c",buff[i]);
//        }
//        printf("%d\n",n);
        int stat;
        printf("pack %u\n",packNum);
        //printf(" %u\n",ntohs(*((unsigned short*)(buff+1))));
//        if(buff[0] == DATA){
//            if(ntohs(*((unsigned short*)(buff+1)))==packNum){
//                fwrite((buff+3), 1, (n-3), file);
//            }
//        }
//        fwrite(tmp,1, strlen(tmp),file);
        fwrite((buff+3), 1, (n-3), file);
        sleep(0.2);
        printf("Done\n");
        ++packNum;
        comfirmMess(sock, buff, 0, packNum);
        if(packNum>=numblock) {
            ok = 1;
            break;
        }
    }
    fclose(file);

    if(!ok) removeFile(cdirectory, filename);
    return 0;
}
int download_mess(int sock, char *buff, char *filename, char *cdirectory, unsigned short numblock){
    char tmp[255];
    strcpy(tmp, cdirectory);
    strcat(tmp, "/");
    strcat(tmp, filename);

    File *file=fopen(tmp, "rb");

    if(file == NULL){
        printf("Cannot read file!");
        return 1;
    }

    buff[0] = CONFIRM;
    *((short*)(buff+1))=htons(numblock);
    send(sock, buff, 3, 0);
    char bufferRecv[BUFF_SIZE];

    int n;
    int packNum=0;

    while(!feof(file)){
        memset(buff,0,BUFF_SIZE);
        memset(bufferRecv,0,BUFF_SIZE);
        ++packNum;
        buff[0] = DATA;
        *((unsigned short*)(buff+1))=htons(packNum);

        n=fread((buff+3), 1, DATA_SIZE, file);
//        for (int i = 0; i < BUFF_SIZE; ++i) {
//            printf("%c",buff[i]);
//        }
//        printf("\n");
        recv(sock, bufferRecv, BUFF_SIZE,0);
        if (bufferRecv[0]==CONFIRM){
                send(sock, buff, n+3, 0);
        }
    }
    fclose(file);
    return 0;
}
int copyfull(char *dest,char *src, int size){
    for (int i = 0; i < size; ++i) {
        dest[i] = src[i];
    }
    return 0;
}
int removeMess(int sock, char *filename, char *cdirectory, char *buffer){
    removeFile(cdirectory,filename);
    return 0;
}