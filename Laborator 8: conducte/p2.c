#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>

#define BSIZE 256

int main(int argc, char ** argv){
    int fd1[2],fd2[2],size,status,i;
    int id, no_prod, no_cons;
    const int no_stars = 10;
    FILE *pipe_in, *pipe_out;
    char ch = '*';
    char buf[BSIZE];
    if(argc != 3){
        perror("Unappropiate number of arguments");
        exit(1);
    }
    if(pipe(fd1) == -1){
        perror("We failed to create the first pipe");
        exit(1);
    }
    if(pipe(fd2) == -1){
        perror("We failed to create the second pipe");
        exit(1);
    }
    no_prod = atoi(argv[1]);
    no_cons = atoi(argv[2]);

    for(i = 0;i < no_cons;++i){
        if((id = fork()) == -1){
            perror("We failed to create the child");
            if(close(fd1[0]) == -1)
                perror("We failed to close the read end of the pipe");
            if(close(fd1[1]) == -1)
                perror("We failed to close the write end of the pipe");
            if(close(fd2[0]) == -1)
                perror("We failed to close the read end of the second pipe");
            if(close(fd2[1]) == -1)
                perror("We failed to close the write end of the second pipe");            
            while(wait(&status) != -1);
            if(errno != 10)
                perror("We failed to wait for all the children");
            exit(1);
        }
        if(id == 0){
            int count = 0;
            int pid = getpid();
            if(close(fd1[1]) == -1){
                perror("We failed to close the write end of the pipe");
                exit(1);
            }
            if(close(fd2[0]) == -1){
                perror("We failed to close the read end of the second pipe");
                exit(1);
            }
            while((size = read(fd1[0],&ch,1)) != 0){
                if(size == -1){
                    perror("We failed to read from the pipe");
                    if(close(fd1[0]) == -1)
                        perror("We failed to close the read end of the pipe");
                    if(close(fd2[1]) == -1)
                        perror("we failed to close the write end of the second pipe");
                    exit(1);
                }
                count += size;
                printf("PID: %d, char: %c\n",pid,ch);
            }
            if(close(fd1[0]) == -1){
                perror("We failed to close the read end of the pipe");
                exit(1);
            }
            if((pipe_out = fdopen(fd2[1],"w")) == NULL){
                if(close(fd2[1]) == -1)
                    perror("we failed to close the write end of the second pipe");
                perror("We failed to map a stream to a file descriptor");
                exit(1);
            }
            fprintf(pipe_out,"PID: %d : %d chars\n",pid,count);
            fflush(pipe_out);
            if(close(fd2[1]) == -1){
                perror("We failed to close the write end of the second pipe");
                exit(1);
            }
            return 0;
        }
    }

    for(i = 1;i < no_prod;++i){
        if((id = fork()) == -1){
            perror("We failed to create the child");
            if(close(fd1[0]) == -1)
                perror("We failed to close the read end of the pipe");
            if(close(fd1[1]) == -1)
                perror("We failed to close the write end of the pipe");
            if(close(fd2[0]) == -1)
                perror("We failed to close the read end of the second pipe");
            if(close(fd2[1]) == -1)
                perror("We failed to close the write end of the second pipe"); 
            while(wait(&status) != -1);
            if(errno != 10)
                perror("We failed to wait for all the children");
            exit(1);
        }
        if(id == 0){
            if(close(fd2[0]) == -1){
                perror("We failed to close the read end of the second pipe");
                exit(1);
            }
            if(close(fd2[1]) == -1){
                perror("We failed to close the write end of the second pipe");
                exit(1);
            }
            if(close(fd1[0]) == -1){
                perror("We failed to close the read end of the pipe");
                exit(1);
            }
            if(dup2(fd1[1],1) == -1){
                perror("We failed to redirect the standard output");
                if(close(fd1[1]) == -1)
                    perror("We failed to close the write end of the pipe");
                exit(1);
            }
            if(close(fd1[1]) == -1){
                perror("We failed to close the write end of the pipe");
                exit(1);
            }
            if(i == 1){
                char * cmd[] = {"ls",NULL};
                execvp(cmd[0],cmd);
                perror("We failed to execute the comand");
                exit(1);
            }
            char * cmd[] = {"./exe",NULL};//the executable must exist
            execvp(cmd[0],cmd);
            perror("We failed to execute the comand");
            exit(1);
        }
    }

    if(close(fd1[0]) == -1){
        perror("We failed to close the read end of the pipe");
        exit(1);
    }
    if(close(fd2[1]) == -1){
        perror("We failed to close the write end of the second pipe");
        exit(1);
    }
    for(i = 0; i < no_stars;++i){
        if(write(fd1[1],&ch,1) == -1){
            perror("We failed to write in the pipe");
            if(close(fd1[1]) == -1)
                perror("We failed to close the write end of the pipe");         
            if(close(fd2[0]) == -1)
                perror("We failed to close the read end of the pipe");
            exit(1);
        }
    }
    if(close(fd1[1]) == -1){
        perror("We failed to close the write end of the pipe");
        exit(1);
    }
    while((size = read(fd2[0],buf,BSIZE)) > 0){
        if(size == -1){
            perror("We failed to read from the second pipe");
            if(close(fd2[0]) == -1)
                perror("We failed to close the read end of the second pipe");
            exit(1);
        }
        printf("%s",buf);
    }
    if(close(fd2[0]) == -1){
        perror("We failed to close the read end of the second pipe");
        exit(1);
    }
    for(i = 1;i < no_cons+no_prod;++i)
        if(wait(NULL) == -1){
            perror("We failed to receive one child's completion acknowledge");
            exit(1);
        }    
    return 0;
}