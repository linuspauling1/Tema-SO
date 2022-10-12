#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define NOT_NULL 1
#define NO_PROC 19

int main(){
    int id = NOT_NULL;
    int i,status;
    for(i = 0;i < NO_PROC;++i){
        id = fork();
        if(id){
           printf("A murit copilul %d",wait(&status));
           printf(" avand codul de iesire: %d\n\n\n",WEXITSTATUS(status));
        }
        else{
            printf("Copil\nPID: %d\nPID parinte: %d\n",getpid(),getppid());
            exit(i);
        }
    }
    printf("Parinte\nPID: %d\nPID parinte: %d\n",getpid(),getppid());
    return 0;
}