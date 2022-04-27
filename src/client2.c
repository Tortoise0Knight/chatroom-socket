/* client.c */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXLINE 80
#define SERV_PORT 8000
#define LOCALHOST 127.0.0.1
#define REMOTE_SERVER 101.34.86.33

int main(int argc, char *argv[])
{
	struct sockaddr_in servaddr;
	char buf[MAXLINE];
	int sockfd, n;
    
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, "REMOTE_SERVER", &servaddr.sin_addr);
	servaddr.sin_port = htons(SERV_PORT);
/* Info that connection established, useful when debugging */
	connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	printf("Connection established with REMOTE_SERVER\n");
	while (fgets(buf, MAXLINE, stdin) != NULL) {
		write(sockfd, buf, strlen(buf));
		n = read(sockfd, buf, MAXLINE);
		if (n == 0)
			printf("the other side has been closed.\n");
		else
			write(STDOUT_FILENO, buf, n);
	}

	close(sockfd);
	return 0;
}
