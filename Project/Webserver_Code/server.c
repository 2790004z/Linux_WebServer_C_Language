#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include "server.h"
#include "signals.h"
#include "client.h"
#include "socket.h"

// Initialize the maximum number of the files
static int initMaxFiles(void){
	// Resource limit struct
	struct rlimit rl;

	// Get current process maximum file descriptor number
	if (getrlimit(RLIMIT_NOFILE, &rl) == -1){
		perror("getlimit");
		return -1;
	}

	// if current the maximum file descriptor number of the current process does not reach the maximum limit then
	if (rl.rlim_cur < rl.rlim_max){
		// set the limit to max
		rl.rlim_cur = rl.rlim_max;

		if (setrlimit(RLIMIT_NOFILE, &rl) == -1){
			perror("setrlimit");
			return -1;
		}
	}
	return 0;
}

// Initialize server
int initServer(short port){
	// Initialize the maximum number of the files
	if (initMaxFiles() == -1){
		return -1;
	}

	// Initialize signals
	if (initSignals() == -1){
		return -1;
	}

	// Initialize socket
	if (initSocket(port) == -1){
		return -1;
	}
	return 0;
}

// Run server
int runServer(char const* home){
	for( ; ; ){
		// Accept client connection
		int conn = acceptClient();
		if (conn == -1){
			return -1;
		}

		// Client handle
		pthread_t tid;
		pthread_attr_t attr; // thread attribute
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED); // set detach state
		CA* ca = (CA*)malloc(sizeof(CA));
		ca->home = home;
		ca->conn = conn;
		int error = pthread_create(&tid, &attr, client, ca);
		if (error){
			printf("Pthread create: %s.\n", strerror(error));
			return -1;
		}
	}
	return 0;
}

// Terminate server
int terminateServer(void){
	// Terminate socket
	terminateSocket();
	return 0;
}
