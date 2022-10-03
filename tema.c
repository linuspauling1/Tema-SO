#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char* argv[])
{
    if(argc < 2) { //error check
        fprintf(stderr,"Argumente insuficiente.\n");
        exit(1);
    }
    struct stat tampon;
    int stat_error = stat(argv[1], &tampon);
    if(stat_error < 0) { //error check
        perror(NULL);
        exit(2);
    }
    if(!S_ISDIR(tampon.st_mode)) { //error check
        frpintf(stderr,"Argumentul nu este director.\n");
        exit(3);
    }
    int fd_pipe[2];
    int err_pipe = pipe(fd_pipe);
    int in = dup(0);
    if(in < 0) { //error check
        perror(NULL);
        exit(-1);
    }
    int out = dup(1);
    if(out < 0) { //error check
        perror(NULL);
        exit(-2);
    }
    int pid = fork();
    if(pid == 0) { //error check
        int close_err = close(fd_pipe[0]);
        if(close_err < 0) {
            perror(NULL);
            exit(-3);
        }
        char* argumente[4];
        argumente[0] = "ls";
        argumente[1] = "-al";
        argumente[2] = argv[1];
        argumente[3] = NULL;
        int dup2_error = dup2(fd_pipe[1],1);
        if(dup2_error < 0) { //error check
            perror(NULL);
            exit(-7);
        }
        int execvp_err = execvp(argumente[0],argumente);
        if(execvp_err < 0) { //error check
            perror(NULL);
            exit(-11);
        }
        close_err = close(fd_pipe[1]);
        if(close_err < 0) { //error check
            perror(NULL);
            exit(-4);
        }
        dup2_error = dup2(out,1);
        if(dup2_error < 0) { //error check
            perror(NULL);
            exit(-8);
        }
        return 0;
    }
    else if(pid > 0) {
        int close_err = close(fd_pipe[1]);
        if(close_err < 0) { //error check
            perror(NULL);
            exit(-5);
        }
        int dup2_error = dup2(fd_pipe[0],0);
        if(dup2_error < 0) { //error check
            perror(NULL);
            exit(-9);
        }

        char s[256];
        while(fgets(s,256,stdin)) {
            s[strlen(s) - 1] = 0;
            int i;
            char iesire[7][256];
            for(i = 0;i < 7;++i)
                scanf("%s",iesire[i]);
            printf("%s %s %s\n",iesire[6],iesire[4],iesire[2]);
        }

        int wait_err = wait(NULL);
        if(wait_err < 0) { //error check
            perror(NULL);
            exit(-12);
        }
        close_err = close(fd_pipe[0]);
        if(close_err < 0) { //error check
            perror(NULL);
            exit(-6);
        }
        dup2_error = dup2(in,0);
        if(dup2_error < 0) { //error check
            perror(NULL);
            exit(-10);
        }
    }
    else if(pid < 0) { //error check
        perror("Armarea procesului a esuat.");
        exit(-3);
    }
    return 0;
}