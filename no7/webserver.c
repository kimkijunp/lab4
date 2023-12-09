#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 3490
#define QLEN 10
#define BUF_SIZE 1024

void handle_client(int client_socket);
void send_error_response(int client_socket);
void send_message_response(int client_socket);
int main() {
	int server_socket, client_socket;
	struct sockaddr_in server_addr, client_addr;
	socklen_t addr_len;
	
	if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket() failed");
		exit(EXIT_FAILURE);
	}
	
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		perror("bind() failed");
		exit(EXIT_FAILURE);
	}
	
	if (listen(server_socket, QLEN) < 0) {
		perror("listen failed");
		exit(EXIT_FAILURE);
	}

	addr_len = sizeof(client_addr);
	
	printf("Server up and running.\n");
	
	while (1) {
		printf("SERVER: Waiting for contact...\n");
		client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);
		
		if (fork() == 0) {
			close(server_socket);
			handle_client(client_socket);
			close(client_socket);
			exit(0);
		} else {
			close(client_socket);
		}
	}
	
	close(server_socket);
	return 0;
}

void handle_client(int client_socket) {
	char msg[BUF_SIZE];
	int recv_bytes = 0;
	int str_len = 0;
	char method[10];
	
	if ((str_len = read(client_socket, &msg[recv_bytes], BUF_SIZE)) == -1) {
		printf("read() error!\n");
		exit(EXIT_FAILURE);
	}
	
	recv_bytes += str_len;
	
	strcpy(method, strtok(msg, " "));
	
	if (strcmp(method, "GET") != 0) {
		send_error_response(client_socket);
	} else {
		send_message_response(client_socket);
	}
}

void send_error_response(int client_socket) {
	char protocol[] = "HTTP/1.1 400 Bad Request\r\n";
	char server[] = "Server:NetscapeEnterprise/6.0\r\n";
	char content_type[] = "Content-Type:text/html\r\n";
	char html[] = "<html><head>BAD Connection</head><body><H1>Bad Request</H1></body></html>\r\n";
	char end[] = "\r\n";
	
	write(client_socket, protocol, strlen(protocol));
	write(client_socket, server, strlen(server));
	write(client_socket, content_type, strlen(content_type));
	write(client_socket, end, strlen(end));
	write(client_socket, html, strlen(html));

	fflush(fdopen(client_socket, "w"));
}
void send_message_response(int client_socket) {
	char protocol[] = "HTTP/1.1 200 OK\r\n";
	char server[] = "Server:Netscape-Enterprise/6.0\r\n";
	char content_length[] = "Content-Length: 139\r\n";
	char content_type[] = "Content-Type:text/html\r\n";
	char html[] = "<html><head>TnT webserver</head><body><center>\\n<H1>inan and yongjjang</H1>\r\n<img src=\"ksu.jpg\"/>\r\n<H2>prof.Kim</H2><center/></body></html>\r\n";
	char end[] = "\r\n";
	
	write(client_socket, protocol, strlen(protocol));
	write(client_socket, server, strlen(server));
	write(client_socket, content_length, strlen(content_length));
	write(client_socket, content_type, strlen(content_type));
	write(client_socket, end, strlen(end));
	write(client_socket, html, strlen(html));
	
	fflush(fdopen(client_socket, "w"));
}
