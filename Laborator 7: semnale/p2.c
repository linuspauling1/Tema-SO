#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void alarm_routine(int);
void child_routine(int);
void terminate_routine(int);

int id;

int main(){
    if((id = fork()) == -1){
        perror("Nu am putut crea un fiu");
        exit(1);
    }
    if(id == 0){
        struct sigaction sa_terminate;
        memset(&sa_terminate,0x00,sizeof(sa_terminate));
        sa_terminate.sa_handler = terminate_routine;
        if(sigaction(SIGUSR1,&sa_terminate,NULL) == -1){
            perror("Nu s-a putut arma semnalul 2");
            exit(1);
        }
        while(1){
            printf("B\n");
            usleep(500000);
        }
    }
    if(id != 0){
        struct sigaction sa_alarm;
        struct sigaction sa_child;
        memset(&sa_alarm,0x00,sizeof(sa_alarm));
        memset(&sa_child,0x00,sizeof(sa_child));
        sa_alarm.sa_handler = alarm_routine;
        sa_child.sa_handler = child_routine;
        if(sigaction(SIGALRM,&sa_alarm,NULL) == -1){
            perror("Nu s-a putut arma semnalul pentru alarma");
            exit(1);
        }
        if(sigaction(SIGCHLD,&sa_child,NULL) == -1){
            perror("Nu s-a putut arma semnalul pentru copil");
            exit(1);
        }
        alarm(7);
        while(1){
            printf("A\n");
            usleep(500000);
        }

    }
    return 0;
}

void alarm_routine(int signum){
    if(kill(id,SIGUSR1) == -1){
        perror("Nu am putut alerta copilul");
        exit(1);
    }
}

void child_routine(int signum){
    int status;
    if(wait(&status) == -1){
        perror("Nu am putut prelua starea copilului");
        exit(1);
    }
    printf("Copilul a iesit cu codul: %d\n",WEXITSTATUS(status));
    exit(0);
}

void terminate_routine(int signum){
    printf("Am terminat!\n");
    exit(0);
}