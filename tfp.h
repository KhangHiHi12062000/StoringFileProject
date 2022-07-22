//
// Created by khangnt-leo on 01/07/2022.
//

#pragma once
#include "file.h"
#include "server.h"


//Request codes
#define CREATE_FOLDER	'0'
#define LIST_DIR	        '1'
#define RES_LIST_DIR     '2'
#define UPLOAD	        '3'
#define DOWNLOAD	    '4'
#define DATA		    '5'
#define CONFIRM	        '6'
#define DELETE	        '7'
#define ERROR	        '8'
#define EXIT             '9'

#define DATA_BLOCK 4096

int GetRequestData(char *buff, char *opcode, char *filename, char *username, unsigned short *numBlock);
char *eatString(char *buff, char *str);
char *eatByte(char *buff, unsigned short *numBlock);
char *writeString(char *buff, char *str);
char *writeBytes(char *buff, unsigned short *numBlock);
char *writeBytel(char *buff, unsigned long *numBlock);
void create_Folder(char *path);
int copy(char *dest, char *src, int *size);
int copyfull(char *dest,char *src, int size);
int comfirmMess(int sock, char *buff, int size, unsigned short numblock);
int upload_mess(int sock, char *buff, char *filename,char *cdirectory , unsigned short numblock);
int removeMess(int sock, char *filename, char *cdirectory, char *buffer);
