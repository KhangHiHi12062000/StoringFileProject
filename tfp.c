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
    //*numBlock = ((hi<<8)|lo);
    *numBlock = hi * 2^8 + lo;
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

    buff[0] = '6';
    writeBytes(buff+1,numblock);

    send(sock,buff, size,0);
    return 0;
}
int upload_mess(int sock, char *buff, char *filename,char *cdirectory, unsigned short numblock){
    strcpy(buff, cdirectory);
    strcat(buff, "/");
    strcat(buff, filename);

    FILE *file=fopen(buff, "r");

    if(file!=NULL){
        //send(sock,"File already exist.",50,0);
        return 1;
    }

    file=fopen(buff, "w+b");
    if(file==NULL){
        //send(sock, "Write file error.", 50,0);
        return 1;
    }

    int n;
    int ok=0;
    int packNum=0;

    comfirmMess(sock, buff, 0, packNum);
    ++packNum;

    int next = 1;
    while (next){
        int i;
        for(i=0 ; i<5 ; ++i){
            n = recv(sock, buff, BUFF_SIZE, 0);
            if(n == -1){
                continue;
            }
            if(buff[0] != DATA) continue;
            if(ntohs(*((unsigned short*)(buff+1)))==packNum) break;
        }
        if(i==5) break;

        int stat=fwrite(buff+3, 1, n-3, file);
        if(stat!=(n-3)){
            //send(sock, "Error on write in file.", 50, 0);
            break;
        }
        if(n<BUFF_SIZE){
            ok=1;
            break;
        }
        ++packNum;
    }
    fclose(file);

    if(!ok) removeFile(cdirectory, filename);
    return 0;
}
int copyfull(char *dest,char *src, int size){
    for (int i = 0; i < size; ++i) {
        dest[i] = src[i];
    }
    return 0;
}
//char *a(char *bu){
//    return bu+2;
//}
//void main(){
//    char bu[16];
//    memset(bu,0,16);
//    char bu2[16];
//    char *p;
//    p = bu2;
//    memset(bu2,0,16);
//    unsigned short num=300;
//    *((unsigned short *) bu) = htons(num);
//    printf("%u\n",bu[0]);
//    printf("%u\n",bu[1]);
//    printf("%s\n",bu);
//    *(p+1)=bu[0];
//    *(p+2)=bu[1];
//    num = ntohs(*(unsigned short *)(p+1));
//    printf("%u\n",num);
//}