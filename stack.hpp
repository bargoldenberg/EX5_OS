struct _node;
typedef struct _node node;


void POP(int* size);
void PUSH(node* ptr,char* msg,int len,int* size);
char* TOP(node* ptr,int* size);