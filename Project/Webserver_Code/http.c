#include <stdio.h>
#define __USE_GNU
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/syscall.h>
#include "http.h"

// Parse request
int parseRequest(char const* req, HTTP_REQUEST* hreq){
	/* 
	request looks like below
		GET /xxx/xxx HTTP/1.1\r\n
		Host: localhost:8000\r\n
		User-Agent: Mozilla/5.0\r\n
		Accept: text/html\r\n
		Connection: keep-alive\r\n
		\r\n
		...
	 */

	sscanf(req, "%s%s%s", hreq->method, hreq->path, hreq->protocol);        // "GET /xxx/xxx HTTP\r\n" ---> method = GET, path = /xxx/xxx, protocol = HTTP
	
	char* connection = strcasestr(req, "connection");                       // Find the 'connection' in the request and does not care the case
	if (connection != NULL){
		sscanf(connection, "%*s%s", hreq->connection);                  // Connection: keep-alive ---> do not care the 'Connection:', hreq->connection = keep-alive
	}
	printf("Process:%d. Thread:%ld>>>>>> [%s][%s][%s][%s]\n", getpid(), syscall(SYS_gettid), hreq->method, hreq->path, hreq->protocol, hreq->connection); // syscall to get the realthread id
	
	if (strcasecmp(hreq->method, "GET")){                                   // This server only support the GET method
		printf("Process:%d. Thread:%ld>>>>>> Invaild method.\n", getpid(), syscall(SYS_gettid));
		return -1;
	}

	if (strcasecmp(hreq->protocol, "HTTP/1.0") && strcasecmp(hreq->protocol, "HTTP/1.1")){ // This server suport HTTP/1.0 && HTTP/1.1
		printf("Process:%d. Thread:%ld>>>>>> Invaild protocol.\n", getpid(), syscall(SYS_gettid));
		return -1;
	}

	return 0;
}

// Construct respond head
void constructHead(HTTP_RESPOND const* hres, char* head){
	/*
	   HTTP/1.1 200 OK\r\n
	   Sever: Webserver 1.0\r\n
	   Date: Mon, 4 Sep 2023 00:00:00 GMT\r\n
	   Content-Type: text/html\r\n
	   Connection: keep-alive\r\n
	   \r\n
	   ...
	 */

	char dateTime[32];
	time_t now = time(NULL);
	strftime(dateTime, sizeof(dateTime), "%a, %d %b %Y %T GMT", gmtime(&now)); // Format time	%a ----> Mon

	sprintf(head, "%s %d %s\r\n"
			"Server: Eason Webserver 1.0\r\n"
			"Date: %s\r\n"
			"Content-Type: %s\r\n"
			"Content-Length: %ld\r\n"
			"Connection: %s\r\n\r\n", hres->protocol, hres->status, hres->description, dateTime, hres->type, hres->length, hres->connection);
}
