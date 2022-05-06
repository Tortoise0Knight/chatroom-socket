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
	char servaddrstr[INET_ADDRSTRLEN];
	int sockfd, n;
    
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, "LOCALHOST", &servaddr.sin_addr);
	servaddr.sin_port = htons(SERV_PORT);
/* Info that connection is being established */
	printf("Establishing connection with %s\n", 
/* Info that connection established, useful when debugging */
	inet_ntop(AF_INET, &servaddr.sin_addr, servaddrstr, sizeof(servaddrstr)));
	if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		printf("Connection failed, exitting...\n");
		return -1;
	}
	else
		printf("Connection established with %s\n", 
	inet_ntop(AF_INET, &servaddr.sin_addr, servaddrstr, sizeof(servaddrstr)));
	while (scanf("%s", buf) != NULL) {
		printf("%s",buf);
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
