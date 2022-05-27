#include <stdio.h>
#include "stack.hpp"
typedef struct _node{
    char data[1024];
    size_t size;
 }node;


void PUSH(node* ptr,char* msg,int len,int* size){
    printf("in push\n");
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

