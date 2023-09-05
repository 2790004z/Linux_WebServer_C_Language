#ifndef _SERVER_H
#define _SERVER_H

// Initialize server
int initServer(short port);

// Run server
int runServer(char const* home);

// Terminate server
int terminateServer(void);

#endif
