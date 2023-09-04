#ifndef _HTTP_H
#define _HTTP_H

#include <limits.h>
#include <sys/types.h>

// HTTP request
typedef struct tag_HttpRequest{
	char method[32];        // Method: GET, POST...
	char path[PATH_MAX+1];  // Path of the file or directory
	char protocol[32];      // Protocol: HTTP, HTTPS...
	char connection[32];    // Connection mode: keep-alive, close
} HTTP_REQUEST;

// HTTP respond
typedef struct tag_HttpRespond{
	char protocol[32];      // Protocol: HTTP, HTTPS...
	int status;             // Status: 404, 200...
	char description[256];       // Describe the status: 404 ---> Not Found
	char type[32];          // Resource type
	off_t length;           // off_t: signed long
	char connection[32];    // Connection mode
} HTTP_RESPOND;

// Parse request
int parseRequest(char const* req, HTTP_REQUEST* hreq);

// Construct repond head
void constructHead(HTTP_RESPOND const* hres, char* head);

#endif
