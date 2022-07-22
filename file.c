//
// Created by khangnt-leo on 30/06/2022.
//
#include "file.h"
#include "server.h";


void addToStack(File **root, char *name, char isDir, char isExec, unsigned long size){
    File *f=(File*)malloc(sizeof(File));
    strncpy(f->name, name, NAMELEN);
    f->size=size;
    f->isDir=isDir;
    f->isExec=isExec;
    f->next=(*root);
    *root=f;
}

File* listFiles(char *path, unsigned short *numFile){
    DIR *dir=opendir(path);

    if(dir==NULL) return NULL;

    File *stack=NULL;
    struct dirent *ent;
    char fullPath[NAMELEN];
    while((ent=readdir(dir))!=NULL){
        if(ent->d_name[0]=='.') continue;
        snprintf(fullPath, NAMELEN-1, "%s/%s", path, ent->d_name);

        if(isDir(fullPath)) addToStack(&stack, ent->d_name, 1, 0, 0);
        else addToStack(&stack, ent->d_name, 0,isExecutable(fullPath) , fileSize(fullPath));
        *numFile += 1;
    }

    closedir(dir);
    return stack;
}

void freeFiles(File *files){
    if(files==NULL) return;

    do{
        File *f=files->next;
        free(files);
        files=f;
    }while(files!=NULL);
}

long fileSize(char *path){
    struct stat st;
    if(stat(path, &st)<0) return -1;
    return st.st_size;
}

int isDir(char *path){
    struct stat st;
    if(stat(path, &st)<0) return -1;
    return S_ISDIR(st.st_mode);
}

int isExecutable(char *path){
    struct stat st;
    if(stat(path, &st)<0) return -1;
    return st.st_mode & S_IXUSR;
}

int dirExist(char *dir){
    DIR* d=opendir(dir);
    if(d){
        closedir(d);
        return 1;
    }else{
        return 0;
    }
}

void sizeToH(unsigned long size, char *buff, size_t max_len){
    char mj[3]={0, 'B', 0};
    double s;

    if(size>1000000000){
        s=size/1000000000;
        mj[0]='G';

    }else if(size>1000000){
        s=size/1000000;
        mj[0]='M';

    }else if(size>1000){
        s=size/1000;
        mj[0]='k';

    }else{
        s=size;
        mj[0]=' ';
    }

    snprintf(buff, max_len, "%4.1f %s", s, mj);
}
void createNewFolder(char *path){
    //mkdir(path,  S_IRWXU | S_IRWXG | S_IRWXO);
    //int result = mkdir(path, 0777);
    errno = 0;
    int ret = mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
    if (ret == -1) {
        switch (errno) {
            case EACCES :
                printf("the parent directory does not allow write");
                exit(EXIT_FAILURE);
            case EEXIST:
                printf("pathname already exists");
                exit(EXIT_FAILURE);
            case ENAMETOOLONG:
                printf("pathname is too long");
                exit(EXIT_FAILURE);
            default:
                perror("mkdir");
                exit(EXIT_FAILURE);
        }
    }
}
int removeFile(char* cdir, char *filename){
    char tmp[256];
    memset(tmp,0,255);
    strcpy(tmp, cdir);
    strcat(tmp, "/");
    strcat(tmp, filename);

    int ret=remove(tmp);
    return ret;
}