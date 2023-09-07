# **Linux Web Server (C Language)**

### I.	Project Introduction

The web server provides static page (HTML) resources, the transport layer is based on the TCP protocol, and the application layer is based on the HTTP protocol.
The browser sends an HTTP request, and the server parses the request after receiving it; parses out the resource path requested by the browser, verifies whether the resource is legal and confirms its type, obtains the resource content locally, organizes it into an HTTP response and sends it back to the web server. The web server supports concurrent processing (multi-threading).

Makefile can provide quick compilation and generation of executable files.

```bash
# Quickly compile and generate .o files and executable files
$ make
# Quickly remove all of the .o files and executable files
$ make clean
```



### II. Project Structure

1. ##### Diagram

![](C:\Users\ROG\Desktop\project-diagram.png)

2. ##### Module Introduction

   ###### a. HTTP Module ( http.h, http.c) 

     - Functionality

       Parse HTTP requests: 

       After the HTTP request sent from the client is received by the server, important data to be used 	later must be extracted from the request and stored.

       Construct HTTP respond:
   
       After receiving the client's HTTP request, the server sends a response back to the client. The response is divided into two parts: the response header and the response body. The response header is completed by this module.
   
   		- Function and Variable:
   
   ```c
   // HTTP request
   typedef struct tag_HttpRequest {...} HTTP_REQUEST;
   // HTTP respond
   typedef struct tag_HttpRespond {...} HTTP_RESPOND;
   // Parse request
   int parseRequest(char const* req, HTTP_REQUEST* hreq);
   // Construct repond head
   void constructHead(HTTP_RESPOND const* hres, char* head);
   ```
   
   ###### b. Resource Module (resource.h, resource.c, mime.h)
   
     - Functionality:
   
       Determine whether the file exists and the file type. If the file can not be found in server, then server send the 404 respond to client.
   
     - Function and Variable:
   
       ```c
       // Filename Suffix: Content-Type Mapping Table
       static struct{...} s_minme[] = {...};
       // Search resource
       int searchResource(char const* path);
       // Identify file types
       int identifyType(char const* path, char* type);
       ```
   
   ###### c. Socket Module (socket.h, socket.c)
   
     - Functionality
   
       Initializing the socket: The server transport layer uses the TCP protocol. When setting up the server, it needs to create sockets, organize address structures, bind, listen and other functions.
   
       Waiting for and receiving the client's connection request: When the client needs to access the server, it needs to complete a three-way handshake. The server needs to wait for the arrival of the connection request, complete the three-way handshake, and establish a communication connection with the client.
   
       Receive the client's HTTP request: After the server and the client establish a communication connection, the client sends an HTTP request to the server. The server needs to receive the request and complete the storage for subsequent operations.
   
       Send response header: The response header is constructed by the HTTP module. In this module, it is responsible for sending the constructed response header to the client.
   
       Sending the response body: The response body is the specific file content requested by the client. The server needs to obtain the file locally, read the content of the file, and send it to the client.
   
       Close the socket.
   
     - Function and Variable:
   
   ```C
   // Initialze socket
   int initSocket(short port);
   // Accept client connection
   int acceptClient(void);
   // Receive request
   char* recvRequest(int conn);
   // Send respond head
   int sendHead(int conn, char const* head);
   // Send respond Body
   int sendBody(int conn, char const* path);
   // Terminate socket
   void terminateSocket(void);
   ```
   
   ###### d. Thread Module (client.h, client.c)
   
    - Functionality
   
      This module is actually a process function of threads, and each thread is responsible for communicating with a client.
   
      Receive requests from clients, parse the content of the requests, and obtain key information such as resource paths. Determine the resource requested by the client to see if it exists locally. If it does not exist, respond to the client with a 404.html file and determine the file type requested by the client. Construct the response header and complete the return of the response header and response body.
   
    - Function and variable:
   
      ```c
      // Client parameter
      typedef struct tag_ClientArgs{...} CA;
      // Client thread process
      void* client(void* arg);
      ```
   
   ###### e. Signals Module (signals.h, signals.c)
   
    - Functionality
   
      In this module, most signals should be ignored (not blocked) to improve the server's working ability and efficiency. Signals that are not ignored: SIGINT(2) and SIGTERM(15) are used for control, SIGKILL(9) and SIGSTOP(19) cannot be ignored, and signals 32 and 33 do not exist.
   
    - Function and varaiable:
   
      ```c
      // Initialze signals
      int initSignals(void);
      ```
   
   ###### f. Server Module (server.h, server.c)
   
    - Functionality
   
      This module is mainly responsible for receiving connection requests from clients and creating threads.
   
      Initialize the server: mainly responsible for completing the initialization of the server, including creating sockets, ignoring most signals, etc.
   
      Running server: It is mainly responsible for cyclically receiving connection requests from clients and establishing communication connections, obtaining communication sockets, creating threads, passing communication sockets into threads, and completing communication with clients.
   
      Terminate the server.
   
    - Function and variable:
   
      ```c
      // Initialize the maximum number of file descriptors
      static int initMaxFiles(void);
      // Initialize the server
      int initServer(short port);
      // Run server
      int runServer(char const* home);
      // Terminate server
      void terminateServer(void);
      ```
   
   ###### g. Main Module (main.c)
   
    - Functionality
   
      This module is the last module implemented in the project, that is, the main function. In the main function, the functions of the server module are called to complete the initialization and running functions of the server. In this module, you can specify the port and resource path bound to the server based on the content passed by the command line parameters. If the command line arguments are missing, the default port number and home directory will be used.
   
    - Function and variable:
   
      ```c
      // main
      int main(int argc, char* argc[]);
      ```
   
   
   
   #### III. Test
   
   
