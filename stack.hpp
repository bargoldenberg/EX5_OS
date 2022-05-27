struct _node;
typedef struct _node node;


void POP(node** ptr);
void PUSH(node** ptr,char* msg,int len);
char* TOP(node** ptr);