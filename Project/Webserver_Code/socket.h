#ifndef _SOCKET_H
#define _SOCKET_H

// Initialize socket
int initSocket(short port);

// Accept client connect
int acceptClient(void);

// Accept request
char* recvRequest(int conn);

// Send respond head
int sendHead(int conn, char const* head);

// Send respond body
int sendBody(int conn, char const* path);

// Terminate socket
void terminateSocket(void);

#endif
