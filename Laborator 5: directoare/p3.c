#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>

#define ALPHA 26 //numarul de litere
#define PSIZE 256 //lungimea maxima a unei cai
#define SMAX 256 //lungimea maxima a unui string
#define FMAX 256 //lungimea maxima a unui fisier
#define BMAX 128 //lungimea maxima a unui bloc

char buffer[FMAX];
char msg[BMAX];
char text[FMAX];
char histograma[ALPHA];

void traversal(const char *);
void writeString(const char *,const char *);
void textToBeWritten();

int main(int argc, char ** argv){
    if(argc != 4){
        fprintf(stderr,"Numar necorespunzator de argumente!\n");
        exit(1);
    }
    traversal(argv[1]);
    writeString(buffer,argv[2]);
    textToBeWritten();
    writeString(text,argv[3]);
    return 0;
}

void traversal(const char * path){
    DIR * dirp;
    struct dirent * entry;
    struct stat buf;
    char full[PSIZE];
    char * tmp;
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
        sprintf(msg,"%s",full);
        if(lstat(full,&buf) == -1){
            perror("Nu se poate studia fisierul curent");
            exit(1);
        }
        if(S_ISDIR(buf.st_mode))
            strcat(msg," directory\n");
        else if(S_ISREG(buf.st_mode))
            strcat(msg," regular\n");
        else if(S_ISLNK(buf.st_mode))
            strcat(msg," symbolic link\n");
        else if(S_ISCHR(buf.st_mode))
            strcat(msg," character device\n");
        else if(S_ISBLK(buf.st_mode))
            strcat(msg," block device\n");
        else if(S_ISFIFO(buf.st_mode))
            strcat(msg," fifo\n");
        else if(S_ISSOCK(buf.st_mode))
            strcat(msg," socket\n");
        else
            strcat(msg," other file\n");
        strcat(buffer,msg);
        if(S_ISDIR(buf.st_mode))
            traversal(full);
        if(S_ISREG(buf.st_mode) && !strcmp(tmp+strlen(tmp)-strlen(".txt"),".txt")){
            int fd;
            int size;
            char s[SMAX];
            if((fd = open(full,O_RDONLY)) == -1){
                perror("Nu s-a putut deschide pentru citire fisierul text");
                exit(1);
            }
            while((size = read(fd,s,SMAX)) != 0){
                if(size == -1){
                    perror("Nu s-a putut citi fisierul");
                    exit(1);
                }
                for(int i = 0;i < size;++i)
                    if(islower(s[i]))
                        ++histograma[s[i]-'a'];
                    else if(isupper(s[i]))
                        ++histograma[s[i] - 'A'];
            }
            if(close(fd) == -1){
                perror("Nu s-a putut inchide fisierul citit");
                exit(1);
            }
        }
    }
    if(closedir(dirp) == -1){
        perror("Nu am putut inchide directorul");
        exit(1);
    }
}

void writeString(const char * string, const char * path){
    int fd;
    if((fd = open(path,O_WRONLY | O_CREAT | O_TRUNC, 0777)) == -1){
        perror("Nu am putut deschide fisierul pentru scriere");
        exit(1);
    }
    if(write(fd,string,strlen(string)) == -1){
        perror("Nu am putut scrie in fisier");
        exit(1);
    }
    if(close(fd) == -1){
        perror("Nu am putut inchide fisierul in care am scris");
        exit(1);
    }
}

void textToBeWritten(){
    int i;
    char out[BMAX];
    for(i = 0;i < ALPHA;++i)
        if(histograma[i]){
            sprintf(out,"%c: %d\n",i+'a',histograma[i]);
            strcat(text,out);
        }
}