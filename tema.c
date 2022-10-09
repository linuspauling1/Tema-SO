#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define MAX_LENGTH 256

void prelucrareRezultatePipe();

int main(int argc, char* argv[]) {
    struct stat tampon;
    if(argc < 2) {
        fprintf(stderr,"Argumente insuficiente!\n");
        exit(1);
    }
    if(argc > 2) {
        fprintf(stderr,"Argumente excesive!\n");
        exit(2);
    }
    if(stat(argv[1],&tampon) == -1) {
        perror("Eroare la analiza fisierului");
        exit(3);
    }
    if(!S_ISDIR(tampon.st_mode)) {
        fprintf(stderr,"Fisierul oferit ca argument nu este director!\n");
        exit(4);
    }
    int fd[2];
    if(pipe(fd) == -1) {
        perror("Eroare la crearea pipe-ului");
        exit(5);
    }
    int pid = fork();
    if(pid == -1) {
        perror("Eroare la crearea procesului fiu");
        exit(6);
    }
    if(pid == 0) {
        if(close(fd[0]) == -1) {
            perror("Eroare la close");
            exit(7);
        }
        char * arguments[4];
        arguments[0] = "ls";
        arguments[1] = "-al";
        arguments[2] = argv[1];
        arguments[3] = NULL;
        if(dup2(fd[1],STDOUT_FILENO) == -1) {
            perror("Eroare la dup2: ");
            exit(8);
        }
        if(execvp(arguments[0],arguments) == -1) {
            perror("Nu a functionat executia...");
            if(close(fd[1]) == -1){
                perror("Eroare la close");
                exit(-1);
            }
            exit(9);
        }
    } else {
        if(close(fd[1]) == -1) {
            perror("Eroare la close");
            exit(10);
        }
        int tmp = dup(STDIN_FILENO);//pregatire restaurare STDIN
        if(tmp == -1) {
            perror("Eroare la dup");
            exit(11);
        }
        if(dup2(fd[0],STDIN_FILENO) == -1) {
            perror("Eroare la dup2");
            exit(12);
        }
        prelucrareRezultatePipe();
        if(close(fd[0]) == -1) {
            perror("Eroare la close");
            exit(13);
        }
        if(dup2(tmp, STDERR_FILENO) == -1) {//restaurarea descriptorului STDIN
            perror("Eroare la dup2");
            exit(14);
        }
        int wstatus;
        if(wait(&wstatus) == -1) {
            perror("Eroare la wait");
            exit(15);
        }
        if(WIFEXITED(wstatus)) {
            int exit_status = WEXITSTATUS(wstatus);
            if(exit_status == 0)
                printf("Comanda a rulat cu succes!\n");//optional
            else
                fprintf(stderr,"Comanda NU a rulat...\n");
        }
    }
    return 0;
}

void prelucrareRezultatePipe() {
    char s[MAX_LENGTH];
    while(fgets(s,MAX_LENGTH,stdin)) {
        s[strlen(s) - 1] = 0;
        int i;
        char iesire[7][MAX_LENGTH];
        for(i = 0;i < 7;++i)
            scanf("%s",iesire[i]);
        printf("%s %s %s\n",iesire[6],iesire[4],iesire[2]);
    }
}