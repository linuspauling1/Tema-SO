#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define PSIZE 256
#define SSIZE 256

void traversal(const char *, int);

int main(int argc, char ** argv){
    if(argc != 3){
        fprintf(stderr,"Numar ilegal de argumente!\n");
        exit(1);
    }
    //presupunem ca al doilea parametru este un numar intreg
    traversal(argv[1], atoi(argv[2]));
    return 0;
}

void traversal(const char * path, int no){
    DIR * dirp;
    struct dirent * entry;
    struct stat buf;
    char * tmp;
    char full[PSIZE];
    if((dirp = opendir(path)) == NULL){
        fprintf(stderr,"Eroare la deschiderea direcotrului %s: %s\n",path,strerror(errno));
        exit(1);
    }
    while((entry = readdir(dirp)) != NULL){
        tmp = entry->d_name;
        if(!strcmp(tmp,".") || !strcmp(tmp,".."))
            continue;
        strcpy(full,path);
        strcat(full,"/");
        strcat(full,tmp);
        if(lstat(full,&buf) == -1){//analizeaza fisierul curent, NU LEGATURA!
            perror("Eroare la analiza intrarii curente");
            exit(1);
        }
        if(S_ISDIR(buf.st_mode))
            traversal(full,no);
        if(strcmp(tmp+strlen(tmp)-strlen(".txt"),".txt"))
            continue;
        if(S_ISREG(buf.st_mode)){
            int fd, size, count = 0;
            char string[SSIZE];
            if((fd = open(full,O_RDONLY)) == -1){
                perror("Nu am putut deschide fisierul text pentru citire");
                exit(1);
            }
            while((size = read(fd,string,SSIZE)) != 0){
                if(size < 0){
                    perror("Nu se poate citi din fisierul text");
                    exit(1);
                }
                for(int i = 0;i < size;++i)
                    if(string[i] == ' ')
                        ++count;
            }
            if(close(fd) == -1){
                perror("Nu am putut inchide fisierul text");
                exit(2);
            }
            if(count > no){
                char slink[PSIZE];
                strcpy(slink,full);
                strcat(slink,"_spaces");
                if(symlink(full,slink) == -1)
                    perror("Nu s-a putut crea legatura simbolica");
            }
        }
    }
    if(closedir(dirp) == -1){
        fprintf(stderr,"Eroare la inchiderea directorului %s: %s\n",path,strerror(errno));
        exit(1);
    }   
}