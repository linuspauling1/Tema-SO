#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define NMAX 256

int main(int argc, char** argv) {
    if(argc != 3) {
        fprintf(stderr,"Numar ilegal de argumente!\n");
        exit(1);
    }
    int fd;
    if((fd = open(argv[1],O_RDONLY)) == -1) {
        perror("Eroare la deschiderea fisierului de intrare");
        exit(2);
    }
    int no = 0;
    int sum = 0;
    unsigned char buf1[2];
    unsigned char buf2[4];
    unsigned char count[] = {0,0,0};
    unsigned int min = -1;
    unsigned int max = 0;
    while(read(fd,buf1,2)) {
        if(errno) {
            perror("Eroare la citirea header-ului");
            exit(3);
        }
        if(buf1[0] != 0x01) {
            fprintf(stderr,"Nu ne-am incadrat bine!\n");
            exit(4);
        }
        if(buf1[1] == 0x01) {
            ++count[0];
            read(fd,buf2,1);
            no = buf2[0];
        }
        else if(buf1[1] == 0x02) {
            ++count[1];
            read(fd,buf2,2);
            no = (buf2[0] << 8) | buf2[1];
        }
        else if(buf1[1] == 0x04) {
            ++count[2];
            read(fd,buf2,4);
            no = (buf2[0] << 24) | (buf2[1] << 16) | (buf2[2] << 8) | buf2[3];
        }
        else {
            fprintf(stderr,"Header neinterpretabil\n");
            exit(5);
        }
        if(errno){
            perror("Eroare la citirea continutului");
            exit(6);
        }
        if(no < min)
            min = no;
        if(no > max)
            max = no;
        sum += no;
    }
    if(close(fd) == -1) {
        perror("Eroare la inchiderea fisierului de intrare");
        exit(7);
    }
    char text[NMAX];
    sprintf(text,"\
    \nMinimum integer: %u\
    \nMaximum integer: %u\
    \nAverage integer: %u\
    \nNumber of 1 byte integers: %u\
    \nNumber of 2 byte integers: %u\
    \nNumber of 4 byte integers: %u\
    \n",
    min,max,sum/(count[0]+count[1]+count[2]),count[0],count[1],count[2]);
    if((fd = open(argv[2],O_WRONLY | O_CREAT,0777)) == -1) {
        perror("Eroare la deschiderea fisierului de iesire");
        exit(8);
    }
    if(write(fd,text,strlen(text) + 1) == -1){
        perror("Eroare la scrierea in fisierul de iesire");
        exit(9);
    }
    if(close(fd) == -1) {
        perror("Eroare la inchiderea fisierului de iesire");
        exit(10);
    }
    return 0;
}