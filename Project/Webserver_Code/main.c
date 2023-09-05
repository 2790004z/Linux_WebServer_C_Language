#include <stdio.h>
#include <stdlib.h>
#include "server.h"

int main(int argc, char* argv[]){
	// Initialize server
	if (initServer(argc < 2 ? 8000 : atoi(argv[1])) == -1){
		return EXIT_FAILURE;
	}

	// Run server
	if (runServer(argc < 3 ? "../WebServer_Home" : argv[2]) == -1){
		return EXIT_FAILURE;
	}

	// Terminate server
	return EXIT_SUCCESS;
}
