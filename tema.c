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
    #include "errors/e1.h"
    int out = dup(1);
    #include "errors/e2.h"
    int pid = fork();
    if(pid == 0) { //error check
        int close_err = close(fd_pipe[0]);
        #include "errors/e3.h"
        char* argumente[4];
        argumente[0] = "ls";
        argumente[1] = "-al";
        argumente[2] = argv[1];
        argumente[3] = NULL;
        int dup2_error = dup2(fd_pipe[1],1);
        #include "errors/e7.h"
        int execvp_err = execvp(argumente[0],argumente);
        #include "errors/e11.h"
        close_err = close(fd_pipe[1]);
        #include "errors/e4.h"
        dup2_error = dup2(out,1);
        #include "errors/e8.h"
        return 0;
    }
    else if(pid > 0) {
        int close_err = close(fd_pipe[1]);
        #include "errors/e5.h"
        int dup2_error = dup2(fd_pipe[0],0);
        #include "errors/e9.h"

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
        #include "errors/e12.h"
        close_err = close(fd_pipe[0]);
        #include "errors/e6.h"
        dup2_error = dup2(in,0);
        #include "errors/e10.h"
    }
    else if(pid < 0) { //error check
        perror("Armarea procesului a esuat.");
        exit(4);
    }
    return 0;
}