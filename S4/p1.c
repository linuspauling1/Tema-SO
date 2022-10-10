#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFF_SIZE 256
#define MAX_SIZE 256

int main(int argc, char** argv){
    if(argc != 4){
        fprintf(stderr,"Numar ilegal de argumente!\n");
        exit(1);
    }
    if(strlen(argv[3]) != 1){
        fprintf(stderr,"Al treilea argument este incorect!\n");
        exit(2);
    }
    int fd;
    if((fd = open(argv[1],O_RDONLY)) == -1){
        perror("Nu s-a putut deschide fisierul pentru citire");
        exit(3);
    } 
    struct stat buffer;
    if(fstat(fd,&buffer) == -1){
        perror("Nu s-a putut analiza fisierul");
        exit(4);
    }
    if(!S_ISREG(buffer.st_mode)){
        fprintf(stderr,"Fisierul de intrare nu este unul ordinar!\n");
        exit(5);
    }
    char block[BUFF_SIZE];
    int buff_size = 1;
    int size = 0;
    int small = 0;
    int caps = 0;
    int digits = 0;
    int occurs = 0;
    int dimension = 0;
    while(buff_size){
        if((buff_size = read(fd,&block,BUFF_SIZE)) == -1){
            perror("Eroare la citirea din fisier");
            exit(6);
        }
        size += buff_size;
        int i;
        for(i = 0;i < buff_size;++i){
            if(islower(block[i]))
                ++small;
            else if(isupper(block[i]))
                ++caps;
            else if(isdigit(block[i]))
                ++digits;
            if(block[i] == argv[3][0])
                ++occurs;
        }
    }
    if(close(fd) == -1){
        perror("Nu s-a putut inchide fisierul de intrare");
        exit(7);
    }
    char text[MAX_SIZE];
    sprintf(text,
    "numar litere mici: %d\
    \numar litere mari: %d\
    \nnumar cifre: %d\
    \nnumar aparitii caracater: %d\
    \ndimensiune fisier: %d",
    small, caps, digits, occurs, size);
    if((fd = open(argv[2],O_WRONLY | O_CREAT,0777)) == -1){
        perror("Nu s-a putut deschide fisierul pentru scriere");
        exit(8);
    }
    if((fstat(fd,&buffer)) == -1){
        perror("Nu s-a putut analiza fisierul de iesire");
        exit(9);
    }
    if(!S_ISREG(buffer.st_mode)){
        fprintf(stderr,"Fisierul de iesire nu este unul ordinar!\n");
        exit(10);
    }
    if(write(fd,text,strlen(text)) == -1){//fara terminator
        perror("Nu s-a putut scrie in fisier");
        exit(11);
    }
    if(close(fd) == -1){
        perror("Nu s-a putut inchide fisierul de iesire");
        exit(12);
    }
    return 0;
}