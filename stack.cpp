#include <stdio.h>
#include "stack.hpp"
typedef struct _node{
    char data[1024];
    _node* next;
    _node* prev;
    size_t size;
 }node;


void PUSH(node** ptr,char* msg,int len){
    printf("in push\n");
    for(int i=0;i<len;i++){
        printf("%d\n",i);
        (*ptr)->data[i] = *(msg++);
        
    }
    (*ptr)->next = *ptr+sizeof(node);
    node* tmp;
    tmp = *ptr;
    *ptr = (*ptr)->next;
    (*ptr)->prev = tmp;
}

void POP(node** n){
    *n=(*n)->prev;
}

char* TOP(node** n){
    return (*n)->prev->data;
}

