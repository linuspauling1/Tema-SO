#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define PSIZE 256

void traversal(const char *,const char *);

int main(int argc, char ** argv){
    if(argc != 3){
        fprintf(stderr,"Numar necorespunzator de argumente!\n");
        exit(1);
    }
    traversal(argv[1],argv[2]);
    return 0;
}

void traversal(const char * path, const char * path_new){
    DIR * dirp;
    struct dirent * entry;
    struct stat buf;
    char * tmp;
    char full[PSIZE];
    char full_new[PSIZE];
    int fd;
    if((dirp = opendir(path)) == NULL){
        perror("Nu am putut deschide directorul");
        exit(1);
    }
    while((entry = readdir(dirp)) != NULL){
        tmp = entry->d_name;
        if(!strcmp(tmp,".") || !strcmp(tmp,".."))
            continue;
        strcpy(full,path);
        strcat(full,"/");
        strcat(full,tmp);
        strcpy(full_new,path_new);
        strcat(full_new,"/");
        strcat(full_new,tmp);
        if(lstat(full,&buf) == -1){
            perror("Nu am putut investiga fisierul");
            exit(1);
        }
        if(S_ISLNK(buf.st_mode)){
            if(symlink(full,full_new) == -1){
                perror("Nu s-a putut crea legatura simbolica");
                exit(1);
            }
            continue;
        }
        if(S_ISDIR(buf.st_mode)){
            if(mkdir(full_new,0777) == -1){//TODO: investigat drepturile directorului original
                perror("Nu s-a putut crea directorul");
                exit(1);
            }
            traversal(full,full_new);
            continue;
        }
        if((fd = creat(full_new,0777)) == -1){//TODO: investigat drepturile fisierului original
            perror("Nu s-a putut crea fisierul");
            exit(1);
        }
        if(close(fd) == -1){
            perror("Nu s-a putut inchide fisierul creat");
            exit(1);
        }
    }
    if(closedir(dirp) == -1){
        perror("Nu am putut inchide directorul");
        exit(1);
    }
}