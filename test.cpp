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
#define SIZE (1024+16)*10000
node* ptr = (node*)mmap(NULL, SIZE ,PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0 );
node** stack = &ptr;

int main(){
    PUSH(stack,"Hello World",strlen("Hello World"));
    int pid = fork();
    if(pid==0){
        PUSH(stack,"NewSTRING",9);
    }else{
        wait(NULL);
    }
    printf("%d\n",pid);
    char* mystr = TOP(stack);
    printf("%s\n",mystr);
    if(!strncmp(mystr,"NewSTRING",9)){
        printf("Test Passed\n");
    }else{
        printf("Test Failed\n");
    }
    munmap(stack,SIZE);
}
// int* num = (int*)mmap(NULL, sizeof(int) ,PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0 );
// int main(){
//     *num =30;
//     if(!fork()){
//         *num=40;
//     }else{
//         wait(NULL);
//     }
//     printf("%d",*num);
// }