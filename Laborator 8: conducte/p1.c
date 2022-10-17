#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BSIZE 256
#define ALPHA 26

const char input_file[] = "date.txt";
const char output_file[] = "statistica.txt";

int main(){
    int id1,id2;
    int fd1[2],fd2[2],fd3[2];
    if(pipe(fd1) == -1){
        perror("Nu am putut crea prima conducta");
        exit(1);
    }
    if(pipe(fd2) == -1){
        perror("Nu am putut crea a doua conducta");
        exit(1);
    }
    if(pipe(fd3) == -1){
        perror("Nu am putut crea a treia conducta");
        exit(1);
    }
    if((id1 = fork()) == -1){
        perror("Nu am putut crea primul copil");
        exit(1);
    }
    if(id1)
        if((id2 = fork()) == -1){
            perror("Nu am putut crea al doilea copil");
            exit(1);
        }
    if(id1 && id2){
        if(close(fd1[0]) == -1){
            perror("Eroare la inchiderea capatului de citire din prima conducta");
            exit(1);
        }
        if(close(fd2[0]) == -1){
            perror("Eroare la inchiderea capatului de citire din a doua conducta");
            exit(1);
        }
        if(close(fd2[1]) == -1){
            perror("Eroare la inchiderea capatului de scriere din a doua conducta");
            exit(1);
        }
        if(close(fd3[1]) == -1){
            perror("Eroare la inchiderea capatului de citire din a treia conducta");
            exit(1);
        }
        int fd;
        int size = 0;
        char distinct[BSIZE];
        char buf[BSIZE];
        if((fd = open(input_file,O_RDONLY)) == -1){
            perror("Nu am putut deschide fisierul de intrare");
            exit(1);
        }
        while((size = read(fd,buf,BSIZE)) > 0){
            if(size == -1){
                perror("Eroare la citirea din fisier");
                exit(1);
            }
            if(write(fd1[1],buf,size) == -1){
                perror("Eroare la scrierea in prima conducta");
                exit(1);
            }
        }
        if(close(fd1[1]) == -1){
            perror("Eroare la inchiderea capatului de scriere in prima conducta");
            exit(1);
        }
        char tmp[BSIZE];
        if(read(fd3[0],tmp,BSIZE) == -1){
            perror("Nu am putut citi din teava");
            exit(1);
        }
        printf("%s",tmp);
        if(close(fd3[0]) == -1){
            perror("Eroare la inchiderea capatului de cirire din a treia conducta");
            exit(1);
        }
        if(close(fd) == -1){
            perror("Nu am putut inchide fisierul de intrare");
            exit(1);
        }
        if(wait(NULL) == -1){
            perror("Eroare la interceptarea unui copil");
            exit(1);
        }
        if(wait(NULL) == -1){
            perror("Eroare la interceptarea altui copil");
            exit(1);
        }
    }
    if(id1 == 0){
        if(close(fd1[1]) == -1){
            perror("Eroare la inchiderea capatului de scriere in prima conducta");
            exit(1);
        }
        if(close(fd2[0]) == -1){
            perror("Eroare la inchiderea capatului de citire din a doua conducta");
            exit(1);
        }
        if(close(fd3[0]) == -1){
            perror("Eroare la inchiderea capatului de cirire din a treia conducta");
            exit(1);
        }
        if(close(fd3[1]) == -1){
            perror("Eroare la inchiderea capatului de citire din a treia conducta");
            exit(1);
        }
        int size = 0;
        char buf[BSIZE];
        if(dup2(fd2[1],1) == -1){
            perror("Eroare la duplicarea descriptorului");
            exit(1);
        }
        while((size = read(fd1[0],buf,BSIZE)) > 0){
            if(size == -1){
                perror("Eroare la citirea din prima conducta");
                exit(1);
            }
            for(int i = 0;i < size;++i)
                if(islower(buf[i]))
                    printf("%c",buf[i]);
        }
        if(close(fd1[0]) == -1){
            perror("Eroare la inchiderea capatului de citire din prima conducta");
            exit(1);
        }
        if(close(fd2[1]) == -1){
            perror("Eroare la inchiderea capatului de scriere in a doua conducta");
            exit(1);
        }
    }
    if(id1 != 0 && id2 == 0){
        if(close(fd1[0]) == -1){
            perror("Eroare la inchiderea capatului de citire din prima conducta");
            exit(1);
        }
        if(close(fd1[1]) == -1){
            perror("Eroare la inchiderea capatului de scriere in prima conducta");
            exit(1);
        }
        if(close(fd2[1]) == -1){
            perror("Eroare la inchiderea capatului de scriere in a doua conducta");
            exit(1);
        }
        if(close(fd3[0]) == -1){
            perror("Eroare la inchiderea capatului de cirire din a treia conducta");
            exit(1);
        }
        int fd;
        int size = 0;
        int distinct = 0;
        char buf[BSIZE];
        int freq[ALPHA] = {0};
        while((size = read(fd2[0],buf,BSIZE)) > 0){
            if(size == -1){
                perror("Eroare la citirea din a doua conducta");
                exit(1);
            }
            for(int i = 0;i < size;++i)
                ++freq[buf[i]-'a'];
        }
        if(close(fd2[0]) == -1){
            perror("Erpare la inchiderea capatului de citire din a doua conducta");
            exit(1);
        }
        if((fd = open(output_file,O_WRONLY | O_CREAT | O_TRUNC, 0777)) == -1){
            perror("Eroare la crearea fisierului");
            exit(1);
        }
        int fd_tmp;
        if((fd_tmp = dup(1)) == -1){
            perror("Duplicare eronata");
            exit(1);
        }
        if(dup2(fd,1) == -1){
            perror("Eroare la redirectionare");
            exit(1);
        }
        for(int i = 0;i < ALPHA;++i)
            if(freq[i]){
                ++distinct;
                printf("%c: %d\n",i+'a',freq[i]);
            }
        char tmp[BSIZE];
        sprintf(tmp,"%d\n",distinct);//trebuie trimis catre procesul parinte!!!
        if(write(fd_tmp,tmp,strlen(tmp)) == -1){
            perror("Nu am putut trimite date prin teava");
            exit(1);
        }
        if(close(fd3[1]) == -1){
            perror("Eroare la inchiderea capatului de scriere in a treia conducta");
            exit(1);
        }
        if(close(fd) == -1){
            perror("Eroare la inchiderea fisierului creat");
            exit(1);
        }
    }
    return 0;
}
