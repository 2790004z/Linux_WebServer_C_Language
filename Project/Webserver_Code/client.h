#ifndef _CLIENT_H
#define _CLIENT_H

// Client parameter
typedef struct tag_ClientArgs{
	char const* home;
	int conn;
} CA;

// Client
void* client(void* arg);

#endif
