#include "stack.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/mman.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
node* stack = (node*)mmap(NULL, (1024+16)*10000
,PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0 );
int* size = (int*)mmap(NULL, sizeof(int)
,PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

int main(){
    *size=0;
    PUSH(stack,"Hello World",strlen("Hello World"),size);
    int pid = fork();
    if(pid==0){
        PUSH(stack,"NewSTRING",9,size);
    }else{
        wait(NULL);
    }
    char* mystr = TOP(stack,size);
    if(!strncmp(mystr,"NewSTRING",9)){
        printf("Test Passed\n");
    }else{
        printf("Test Failed\n");
    }
    munmap(stack,(1024+16)*10000);
}

// int* num = (int*)mmap(NULL, sizeof(int)*2 ,PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0 );
// void changenum(int* num){
//     num[0]=33;
// }
// int main(){
//     num[0] =30;
//     if(!fork()){
//         num[1]=40;
//         changenum(num);
//     }else{
//         wait(NULL);
//     }
//     printf("%d\n",num[0]);
//     printf("%d\n",num[1]);
// }