//
// Created by khangnt-leo on 01/07/2022.
//

#pragma once
#include "net.h"

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

//Error codes


int GetRequestData(char *buff, char opcode, char *filename, char *username, unsigned short *numBlock);
char *eatString(char *buff, char *str);
char *eatByte(char *buff, unsigned short *opcode);
int main();

