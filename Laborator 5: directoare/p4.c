#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>

#define PSIZE 256

void traversal(const char *, const char *);

int main(int argc, char** argv){
    if(argc != 3){
        fprintf(stderr,"Numar necorespunzator de argumente!\n");
        exit(1);
    }
    traversal(argv[1],argv[2]);
    return 0;
}

void traversal(const char * path, const char * s){
    DIR * dirp;
    struct dirent * entry;
    struct stat buf;
    char * tmp;
    char full[PSIZE];
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
        if(lstat(full,&buf) == -1){
            perror("Nu s-a putut explora fisierul");
            exit(1);
        }
        if(strstr(tmp,s)){
            if(S_ISLNK(buf.st_mode))
                printf("symbolic link ");
            printf("%s\n",full);
        }
        if(S_ISDIR(buf.st_mode))
            traversal(full,s);
    }
    if(closedir(dirp) == -1){
        perror("Nu am putut inchide directorul");
        exit(1);
    }
}