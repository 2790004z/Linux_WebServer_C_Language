#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include "client.h"
#include "socket.h"
#include "http.h"
#include "resource.h"

// Client
void* client(void* arg){
	CA* ca = (CA*)arg;
	printf("Process:%d. Thread:%ld>>>>>> Client start.\n", getpid(), syscall(SYS_gettid));

	for( ; ; ){
		printf("--------------------------------------------------------------------------------------------------\n");

		printf("Process:%d. Thread:%ld>>>>>> Recieve the request.\n", getpid(), syscall(SYS_gettid));

		char* req = recvRequest(ca->conn);
		if (req == NULL) break;

		printf("Process:%d. Thread:%ld>>>>>> Request body\n\n%s", getpid(), syscall(SYS_gettid), req);

		printf("Process:%d. Thread:%ld>>>>>> Parse the request.\n", getpid(), syscall(SYS_gettid));
		// Parse request
		HTTP_REQUEST hreq = {};
		if (parseRequest(req, &hreq) == -1){
			free(req);
			break;
		}

		free(req);

		// Resource request
		char path[PATH_MAX+1];
		strcpy(path, ca->home);
		if (path[strlen(path)-1] == '/') path[strlen(path)-1] = '\0';
		strcat(path, hreq.path);
		if (!strcpy(hreq.path, "/")) strcat(path, "index.html");

		printf("Process:%d. Thread:%ld>>>>>> Resource path: %s.\n", getpid(), syscall(SYS_gettid), path);

		// Search resource
		HTTP_RESPOND hres = {"HTTP/1.1", 200, "OK", "text/html"};
		if (searchResource(path) == -1){
			hres.status = 404;
			strcpy(hres.description, "Not Found");
			strcpy(path, "../Webserver_Home/404.html");
		} else if (identifyType(path, hres.type) == -1){
			hres.status = 404;
			strcpy(hres.description, "Not Found");
			strcpy(path, "../Webserver_Home/404.html");
		}

		struct stat st;
		if (stat(path, &st) == -1){
			perror("stat");
			break;
		}
		hres.length = st.st_size;

		if (strlen(hreq.connection)) strcpy(hres.connection, hreq.connection); 
		else {
			if (!strcasecmp(hreq.protocol, "HTTP/1.0")) strcpy(hres.connection, "close");
			else strcpy(hres.connection, "keep-alive");
		}

		// Construct respond head
		char head[1024];
		constructHead(&hres, head);

		printf("Process:%d. Thread:%ld>>>>>> Respond head.\n\n%s", getpid(), syscall(SYS_gettid), head);

		printf("Process:%d. Thread:%ld>>>>>> Send respond.\n", getpid(), syscall(SYS_gettid));

		// Send respond head
		if (sendHead(ca->conn, head) == -1) break;

		if (sendBody(ca->conn, path) == -1) break;

		if (!strcasecmp(hres.connection, "close")) break;
	}

	close(ca->conn);
	free(ca);
	printf("Process:%d. Thread:%ld>>>>>> Client closed.\n", getpid(), syscall(SYS_gettid));

	return NULL;
}
