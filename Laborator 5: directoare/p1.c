#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define PSIZE 256

struct links_size {
    int links;
    int size;
};

struct links_size traversal(const char *);
int sizeCur(const char *);

int main(int argc, char ** argv){
    if(argc != 3){
        fprintf(stderr,"Numar ilegal de argumente!\n");
        exit(1);
    }
    FILE* f;
    int fd;
    char path[PSIZE];
    char msg[PSIZE];
    struct links_size tmp;
    if((f = fopen(argv[1],"r")) == NULL){
        perror("Nu am putut deschide fisierul de intrare");
        exit(1);
    }
    if((fd = open(argv[2],O_WRONLY | O_CREAT, 0777)) == -1){
        perror("Nu am putut deschide fisierul de iesire");
        exit(1);
    }
    while(fgets(path,PSIZE,f)){
        if(path[strlen(path)-1] == '\n')
            path[strlen(path)-1] = 0;
        tmp = traversal(path);
        sprintf(msg,"%s %d %d\n",path,tmp.size+sizeCur(path),tmp.links);
        if(write(fd,msg,strlen(msg)) == -1){
            perror("Nu am putut scrie in fisier");
            exit(1);
        }
    }
    if(fclose(f)){
        perror("Nu am putut inchide fisierul de intrare");
        exit(1);
    }
    if(close(fd) == -1){
        perror("Nu am putut inchide fisierul de iesire");
        exit(1);
    }
    return 0;
}

struct links_size traversal(const char * path){
    DIR * dirp;
    struct dirent * cur_file;
    struct stat buf;
    char * tmp;
    char full_path[PSIZE];
    struct links_size ls = {0,0};
    struct links_size out = {0,0};
    if((dirp = opendir(path)) == NULL){
        perror("Nu am putut deschide directorul");
        return ls;
    }
    while((cur_file = readdir(dirp)) != NULL){
        tmp = cur_file->d_name;
        if(!strcmp(tmp,".") || !strcmp(tmp,".."))
            continue;
        strcpy(full_path,path);
        strcat(full_path,"/");
        strcat(full_path,tmp);
        if(lstat(full_path,&buf) == -1){
            perror("Nu am putut analiza intrarea din director");
            exit(1);
        }
        if(S_ISDIR(buf.st_mode))    
            out = traversal(full_path);
        if(S_ISLNK(buf.st_mode))
            ++ls.links;
        ls.links += out.links;
        ls.size += buf.st_size + out.size;
    }
    if(closedir(dirp) == -1){
        perror("Nu am putut inchide directorul");
        exit(1);
    }
    return ls;
}

int sizeCur(const char * path){
    DIR * dirp;
    struct dirent * cur_file;
    struct stat buf;
    if((dirp = opendir(path)) == NULL)
        return 0;
    if(stat(path,&buf) == -1){
            perror("Nu am putut analiza directorul");
            exit(1);
    }
    if(closedir(dirp) == -1){
        perror("Nu am putut inchide directorul");
        exit(1);
    }
    return buf.st_size;
}