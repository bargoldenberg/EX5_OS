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
#define WORDSIZE (1024+16)*10000

//Global pointers for shared mem between proccesses.
node* stack = (node*)mmap(NULL, WORDSIZE ,PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0 );
int* size = (int*)mmap(NULL, sizeof(int) ,PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

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

