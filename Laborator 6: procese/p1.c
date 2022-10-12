#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(){
    int i,id;
    const int nr_procese = 3;
    for(i = 1;i <= nr_procese;++i){
        fork();
        printf("Numara de cate ori ai luat mure!\n");
    }
    while(wait(NULL) != -1);
    return 0;
}