#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BUF_SIZE 136
#define filename 100
#define filemodel 8
#define o_uid 8
#define g_uid 8
#define size 12

int octalToDecimal(int);
int power(int,int);

int main(int argc, char ** argv){
    if(argc != 2){
        fprintf(stderr,"Numar incorect de argumente!\n");
        exit(1);
    }
    if(strcmp(argv[1]+strlen(argv[1])-4,".tar")){
        fprintf(stderr,"Fisierul dat ca argument nu este .tar!\n");
        exit(2);
    }
    int fd;
    if((fd = open(argv[1],O_RDONLY)) == -1){
        perror("Nu s-a putut deschide fisierul pentru citire");
        exit(3);
    }
    //name, size, oid, gid
    char block[BUF_SIZE];
    if(read(fd,block,BUF_SIZE) == -1){
        perror("Nu s-a putut citi din arhiva");
        exit(4);
    }
    if(close(fd) == -1){
        perror("Eroare la inchiderea fisierului");
        exit(5);
    }
    char buf_name[filename];
    char buf_ouid[o_uid];
    char buf_guid[g_uid];
    char buf_size[size];
    strncpy(buf_name,block,filename);
    strncpy(buf_ouid,block+filename+filemodel,o_uid);
    strncpy(buf_guid,block+filename+filemodel+o_uid,g_uid);
    strncpy(buf_size,block+filename+filemodel+o_uid+g_uid,size);
    printf("File name: %s\n",buf_name);
    printf("File size: %d\n",octalToDecimal(atoi(buf_size)));
    printf("File user id: %d\n",octalToDecimal(atoi(buf_ouid)));
    printf("File group id: %d\n",octalToDecimal(atoi(buf_guid)));
    return 0;
}

int octalToDecimal(int number){
    int sum = 0, p = 0;
    while(number){
        sum += power(8,p++)*(number%10);
        number /= 10;
    }
    return sum;
}

int power(int base,int exponent){
    int prod = 1;
    int i;
    for(i = 0;i < exponent;++i)
        prod *= base;
    return prod;
}