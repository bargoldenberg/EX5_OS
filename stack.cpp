#include <stdio.h>
#include "stack.hpp"
#ifdef USE_MAP_ANON
#define _BSD_SOURCE
#endif  
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

typedef struct _node{
    char data[1024];
    size_t size;
 }node;


void PUSH(node* ptr,char* msg,int len,int* size){
    for(int i=0;i<len;i++){
        ptr[*size].data[i] = *(msg++);
        
    }
    *size=*size+1;
}

void POP(int* size){
    *size=*size-1;
}

char* TOP(node* ptr,int* size){
    int indx = *size;
    return ptr[*size-1].data;
}

