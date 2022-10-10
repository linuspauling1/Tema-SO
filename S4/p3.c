#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

#define BSIZE 4
#define CHMAX 64

int main(int argc, char ** argv){
    if(argc != 3){
        fprintf(stderr,"Numar ilegal de argumente!\n");
        exit(1);
    }
    int fd;
    if((fd = open(argv[1],O_RDONLY)) == -1){
        perror("Nu s-a putut deschide fisierul de intrare");
        exit(2);
    }
    struct stat buf;
    if(fstat(fd,&buf) == -1){
        perror("Eroare la analiza fisierului de intrare");
        exit(3);
    }
    int count = 0;
    unsigned int min = -1;
    unsigned int max = 0;
    unsigned int sum = 0;
    unsigned char s_buf[BSIZE];
    while(read(fd,s_buf,BSIZE)){
        if(errno){
            perror("Nu s-a putut efectua citirea din fisier");
            exit(4);
        }
        int x = 0;
        x |= (s_buf[0] << 24) | (s_buf[1] << 16) | (s_buf[2] << 8) | s_buf[3];
        if(x < min)
            min = x;
        if(x > max)
            max = x;
        sum += x;
        ++count;
    }
    if(close(fd) == -1){
        perror("Nu s-a putut inchide fisierul de intrare");
        exit(5);
    }
    if((fd = open(argv[2],O_WRONLY | O_CREAT)) == -1){
        perror("Nu s-a putut deschide fisierul de iesire");
        exit(6);
    }
    char rezultat[CHMAX];
    sprintf(rezultat,"maxim: %u\nminim: %u\nmedie: %u\n",max,min,sum/count);
    if(write(fd,rezultat,strlen(rezultat)) == -1){
        perror("Nu s-a putut scrie in fisier");
        exit(7);
    }
    if(close(fd) == -1){
        perror("Nu s-a putut inchide fisierul de iesire");
        exit(8);
    }
    return 0;
}