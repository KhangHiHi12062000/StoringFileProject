#include "tfp.h"

int GetRequestData(char *buff, char *opcode, char *filename, char *username, unsigned short *numBlock){
    *opcode=buff[0];

    switch (*opcode) {
        case CREATE_FOLDER:
            buff = eatString(buff+1, username);
            printf("\n username : %s", username);
            break;
        case LIST_DIR:
            buff = eatString(buff+1, username);
            break;
        case RES_LIST_DIR:
            break;
        case UPLOAD:
            buff = eatString(buff+1, filename);
            break;
        case DOWNLOAD:
            break;
        case DATA:
            break;
        case CONFIRM:
            buff = eatByte(buff+1, numBlock);
            break;
        case DELETE:
            buff = eatString(buff+1, filename);
            break;
        case ERROR:
            break;
        case EXIT:
            break;
        default:
            printf("\nopcode is not exist!!");
            return 1;
    }
    return 0;
}

char *eatString(char *buff, char *str){
    int i = 0;
    while(buff[i] != '\0' && buff[i] != '|'){
        str[i] = buff[i];
        i++;
    }
    str[i] = '\0';
    return buff+i+1;
}

char *eatByte(char *buff, unsigned short *numBlock){
    unsigned short hi;
    unsigned short lo;

    hi = buff[0];
    lo = buff[1];
    *numBlock = ((hi<<8)|lo);
    printf("%u\n",hi);
    printf("%u\n",lo);
    printf("%u\n",*numBlock);
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

//void main(){
//    char path[500]={'a','b','c'};
//    char *ptr;
//    ptr = path;
//    int p=4;
//    char src1[500]="adfhkhjfdak";
//    char src2[500]="243897298347";
//    int size;
//    copy(ptr+4 ,src1,&size);
//    p = p + size;
//    copy(ptr + p,src2,&size);
//
//    p = p + size;
//    for (int i = 0; i < 34; ++i) {
//        printf("%c",path[i]);
//    }
//    printf("%d",p);
//}