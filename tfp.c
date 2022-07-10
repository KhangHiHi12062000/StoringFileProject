#include "tfp.h"

//int GetRequestData(char *buff, char *opcode, char *filename, char *username, unsigned short *numBlock){
//    *opcode=buff[0];
//
//    if( opcode != CREATE_FOLDER &&
//        opcode != LIST_DIR &&
//        opcode != RES_LIST_DIR &&
//        opcode != UPLOAD &&
//        opcode != DOWNLOAD &&
//        opcode != DATA &&
//        opcode != CONFIRM &&
//        opcode != DELETE &&
//        opcode != ERROR &&
//        opcode != EXIT){
//        printf("opcode is not exist!!\n");
//        return 1;
//    }
//
//    switch (*opcode) {
//        case CREATE_FOLDER:
//            //buff = eatByte(buff, numBlock);
//            //buff = eatString(buff, username);
//            break;
//        case LIST_DIR:
//            buff = eatString(buff, username);
//            break;
//        case RES_LIST_DIR:
//            break;
//        case UPLOAD:
//            buff = eatString(buff, filename);
//            break;
//        case DOWNLOAD:
//            break;
//        case DATA:
//            break;
//        case CONFIRM:
//            break;
//        case DELETE:
//            break;
//        case ERROR:
//            break;
//        case EXIT:
//            break;
//        default:
//            return 1;
//    }
//    return 0;
//}
//
//char *eatString(char *buff, char *str){
//    int i = 0;
//    while(buff[i] != '\0' && buff[i] != '|'){
//        str[i] = buff[i];
//        i++;
//    }
//    str[i] = '\0';
//    return buff+i+1;
//}
//
//char *eatByte(char *buff, unsigned short *opcode){
//
//    *opcode = atoi(buff);
//    int i=1;
//    int x = *opcode/10;
//    while(x>0){
//        x=x/10;
//        i++;
//    }
//    return buff+i+1;
//}
int main(){
//    char buff[512];
//    char opcode;
//    int len = 512;
//    char filename[512];
//    char username[512];
//    unsigned short p = 0;
//    char y[20] = "Hello World";
//    char z[20] = "Thank you";
//    unsigned short k = 323;
//    //sprintf(buff, "%u|%u|%s|%s\n",p,k,y,z);
//    strcpy(buff, y);
//    strcpy(buff+ strlen(buff)+1,z);
//    for(int i=0; i<30;i++){
//        printf("%c",buff[i]);
//    }
//    unsigned short numBlock;
//    GetRequestData(buff, &opcode, filename, username, &numBlock);
//
//    printf("\nbuff %s\n", buff);
//    printf("opcode %c\n", opcode);
//    printf("username %s\n", username);
//    printf("numblock %d\n",numBlock);
    unsigned short hi;
    unsigned short lo;

    unsigned short x=24500;
    lo = x & 0xff;
    hi = (x>>8) & 0xff;
    //x = ((hi<<8)|lo);
    printf("%u\n",x);
    printf("%c\n",hi);
    printf("%c\n",lo);

    return 0;
}