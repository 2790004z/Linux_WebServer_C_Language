#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/syscall.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "socket.h"

static int s_sock = -1; // To listen connect request

// Initialize socket
int initSocket(short port){
	printf("Process:%d>>>>>> Creating socket.\n", getpid());
	s_sock = socket(AF_INET, SOCK_STREAM, 0); // TCP
	if (s_sock == -1){
		perror("socket");
		return -1;
	}
	
	printf("Process:%d>>>>>> Stting the socket.\n", getpid());
	int on = 1;
	if (setsockopt(s_sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1){
		perror("setsockopt");
		return -1;
	}

	printf("Process:%d>>>>>> Binding the port.\n", getpid());
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;           // IPv4
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY; 
	if (bind(s_sock, (struct sockaddr*)&addr, sizeof(addr)) == -1){
		perror("bind");
		return -1;
	}

	printf("Process:%d>>>>>> Listen socket.\n", getpid());
	if (listen(s_sock, 1024) == -1){
		perror("listen");
		return -1;
	}

	return 0;
}

// Accept client connect
int acceptClient(void){
	printf("Process:%d>>>>>> Waiting for client connection.\n", getpid());
	struct sockaddr_in addrcli = {};
	socklen_t addrlen = sizeof(addrcli);
	int conn = accept(s_sock, (struct sockaddr*)&addrcli, &addrlen);
	if (conn == -1){
		perror("accept");
		return -1;
	}

	printf("Process:%d>>>>>> Connected client: %s:%hu.\n", getpid(), inet_ntoa(addrcli.sin_addr), ntohs(addrcli.sin_port));
	return conn;
}

// Accept request
char* recvRequest(int conn){
	char* req = NULL;
	ssize_t len = 0;

	for ( ; ; ){
		char buf[1024] = {};
		ssize_t recvLen = recv(conn, buf, sizeof(buf)-1, 0); // Recieved length
		if (recvLen == -1){
			perror("recv");
			free(req);       // free the memory to prevent memory leakage
			return NULL;
		}

		req = (char*)realloc(req, len+recvLen+1);
		memcpy(req+len, buf, recvLen+1);
		len += recvLen;

		if (strstr(req, "\r\n\r\n")) break;
	}

	return req;
}

// Send respond head
int sendHead(int conn, char const* head){
	if (send(conn, head, strlen(head), 0) == -1){
		perror("send");
		return -1;
	}

	return 0;
}

// Send respond body
int sendBody(int conn, char const* path){
	int fd = open(path, O_RDONLY);
	if (fd == -1){
		perror("open");
		return -1;
	}

	char buf[1024];
	ssize_t len;

	while((len = read(fd, buf, sizeof(buf))) > 0){
		if (send(conn, buf, len, 0) == -1){
			perror("send");
			return -1;
		}
	}
	if (len == -1){
		perror("read");
		return -1;
	}

	close(fd);
	return 0;
}

// Terminate socket
void terminateSocket(void){
	close(s_sock);
}
