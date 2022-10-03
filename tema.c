#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
    int fd_pipe[2];
    int err_pipe = pipe(fd_pipe);
    int in = dup(0);
    int out = dup(1);
    int pid = fork();
    if(pid == 0) {
        close(fd_pipe[0]);
        char* argumente[4];
        argumente[0] = "ls";
        argumente[1] = "-al";
        argumente[2] = argv[1];
        argumente[3] = NULL;
        dup2(fd_pipe[1],1);
        execvp(argumente[0],argumente);
        close(fd_pipe[1]);
        dup2(out,1);
        return 0;
    }
    else if(pid > 0) {
        close(fd_pipe[1]);
        dup2(fd_pipe[0],0);

        char s[256];
        while(fgets(s,256,stdin)) {
            s[strlen(s) - 1] = 0;
            int i;
            char iesire[7][256];
            for(i = 0;i < 7;++i)
                scanf("%s",iesire[i]);
            printf("%s %s %s\n",iesire[6],iesire[4],iesire[2]);
        }

        wait(NULL);
        close(fd_pipe[0]);
        dup2(in,0);
    }
    else if(pid < 0) {
        perror("Armarea procesului a esuat.");
        exit(-1);
    }
    return 0;
}