#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

int number;

void routine1(int);
void routine2(int);
void terminate_routine(int);
void alarm_routine(int);

int main(){
    struct sigaction sa;
    memset(&sa,0x00,sizeof(sa));
    sa.sa_handler = routine1;
    if(sigaction(SIGUSR1,&sa,NULL) < 0){
        perror("User2 issue");
        exit(1);
    }
    sa.sa_handler = routine2;
    if(sigaction(SIGUSR2,&sa,NULL) < 0){
        perror("User1 issue");
        exit(1);
    }
    sa.sa_handler = terminate_routine;
    if(sigaction(SIGTERM,&sa,NULL) < 0){
        perror("Terminate issue");
        exit(1);
    }
    sa.sa_handler = routine1;
    if(sigaction(SIGALRM,&sa,NULL) < 0){
        perror("Alarm issue");
        exit(1);
    }
    while(1){
        printf("%d\n",number);
        usleep(500);
    }
}

void routine1(int sig){
    ++number;
}

void routine2(int sig){
    if(alarm(5) == -1){
        perror("Alarm cannot be triggered");
        exit(1);
    }
}

void terminate_routine(int sig){
    printf("Execution terminated!\n");
    exit(0);
}