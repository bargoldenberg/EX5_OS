/*
** server.c -- a stream socket server demo
*/

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
#include "stack.hpp"
#include <iostream>
#define PORT "3500"  // the port users will be connecting to
using namespace std;
#define WORDSIZE (1024+16)*1000
#define BACKLOG 10	 // how many pending connections queue will hold
//global pointers for shared memory.

node* stack = (node*)mmap(NULL, WORDSIZE ,PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0 );
node* start=stack;
int* size = (int*)mmap(NULL, sizeof(int) ,PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0 );



void sigchld_handler(int s)
{
	(void)s; // quiet unused variable warning

	// waitpid() might overwrite errno, so we save and restore it:
	int saved_errno = errno;

	while(waitpid(-1, NULL, WNOHANG) > 0);

	errno = saved_errno;
}


void *stack_operations(int new_fd){
    printf("IN PROCESS\n");
    char buffer[1024];
    while(1){
        bzero(buffer,sizeof(buffer));//used to reset buffer
        if(recv(new_fd,buffer,1024,0)){//wait for command from client.
            if(!(strncmp(buffer,"PUSH",4))){//strncmp is a special function used to compare a specific amount of bytes between 2 strings.
                char* msg= (char*)malloc(sizeof(char)*1024);
                bzero(msg,1024);
                int k=0;
                for(size_t i=5;i<strlen(buffer)+1;i++){//insert desired message into the stack.
                    msg[k++]=buffer[i];
                }
                PUSH(stack,msg,1024,size);
            }else if(!(strncmp(buffer,"POP",3))){//pop.
                POP(size);
            }else if(!(strncmp(buffer,"TOP",3))){//top.
                char * s  = TOP(stack,size);
                printf("OUTPUT: %s\n",s);
				//printf("start: %s",start->data);
                send(new_fd,s,strlen(s)+1,0);//because top is the only function that returns an output it is sent the client.
            }else if(!(strncmp(buffer,"EXIT",4))){//used to kill thread.
                munmap(start,WORDSIZE);
				munmap(size,sizeof(int));
				break;
            }
        }
    }

}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{
	*size = 0;
	int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
	struct sigaction sa;
	int yes=1;
	char s[INET6_ADDRSTRLEN];
	int rv;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
				sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}

		break;
	}

	freeaddrinfo(servinfo); // all done with this structure

	if (p == NULL)  {
		fprintf(stderr, "server: failed to bind\n");
		exit(1);
	}

	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}

	printf("server: waiting for connections...\n");

	while(1) {  // main accept() loop
		sin_size = sizeof their_addr;
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1) {
			perror("accept");
			continue;
		}

		inet_ntop(their_addr.ss_family,
			get_in_addr((struct sockaddr *)&their_addr),
			s, sizeof s);
		printf("server: got connection from %s\n", s);
		if (!fork()) { // this is the child process
			stack_operations(new_fd); // child doesn't need the listener
		}
		close(new_fd);  // parent doesn't need this
	}

	return 0;
}
