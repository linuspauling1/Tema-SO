#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>

#define NO_THREADS 5
#define NO_TIMES 10000
#define NO_SECS 100000

void* thread(void*);

char tensor[] = "#####";
int th[NO_THREADS],th_no[NO_THREADS];

int main(){
    srand(time(NULL));
    int flag,i;
    pthread_t tid[NO_THREADS];
    for(i = 0;i < NO_THREADS;++i){
        th_no[i] = i;
        flag = pthread_create(&tid[i],NULL,thread,(void*)(&th_no[i]));
        if(flag){
            puts(strerror(flag));
            exit(1);
        }
    }
    while(!(th[0] && th[1] && th[2] && th[3] && th[4]))
        puts(tensor);
    for(i = 0;i < NO_THREADS;++i){
        flag = pthread_join(tid[i],NULL);
        if(flag){
            puts(strerror(flag));
            exit(1);
        }
    }
    return 0;
}

void* thread(void * array){
    int identifier = *((int*)array);
    char fill = 'A' + identifier;
    int number = (identifier+1)*NO_TIMES;
    while(number--)
        tensor[rand()%5] = fill;
    th[identifier] = 1;
    return NULL;
}