/* client.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXLINE 80
#define SERV_PORT 8000

int main(int argc, char *argv[])
{
	struct sockaddr_in servaddr;
	char buf[MAXLINE];
	int sockfd, n;
	char *str;
    
	if (argc != 2) {
		fputs("usage: ./client message\n", stderr);
		exit(1);
	}
/* Pass the input string argument */  
	str = argv[1];
/* Initialize the clientfd */    
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
/* Specify the server address */    
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
	servaddr.sin_port = htons(SERV_PORT);
/* Connect to server */     
	connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
/* Send msg from string */ 
	write(sockfd, str, strlen(str));
/* Receive server response and print to stdio*/ 
	n = read(sockfd, buf, MAXLINE);
	printf("Response from server:\n");
	write(STDOUT_FILENO, buf, n);
	printf("\n");

	close(sockfd);
	return 0;
}
