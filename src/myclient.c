/* client.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAXLINE 80
#define SERV_PORT 8000
#define LOCALHOST 127.0.0.1
#define REMOTE_SERVER 101.34.86.33

void* checkmsg(void* sfd){
	char buf[MAXLINE];
	int sockfd = *(int*)sfd;
	int n;
	while (1) {
		/*printf("%s",buf);*/
		scanf("%s", buf);
		send(sockfd, buf, strlen(buf),0);
		printf("send the buf: %s\n",buf);
		/*n = read(sockfd, buf, MAXLINE);
		if (n == 0)
			printf("the other side has been closed.\n");
		else
			write(STDOUT_FILENO, buf, n);*/
	}
}

//clean the buf
void cleanout(char* buf)
{
	int n = strlen(buf);
	for(int i = 0; i < n; i++)
	{
		buf[i] = 0;
	}
}

int main(int argc, char *argv[])
{
	struct sockaddr_in servaddr;
	char buf[MAXLINE];
	char servaddrstr[INET_ADDRSTRLEN];
	int sockfd, n, a;
    
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
	/*while(1)
	{
		if(scanf("%s",buf)!=NULL)
		{
			write(sockfd, buf, strlen(buf));
		}
		if()
	}*/
	pthread_t tid;
    pthread_create(&tid,0,checkmsg,&sockfd);
	while(1){
		cleanout(buf);
		if (recv(sockfd,buf,sizeof(buf),0) <= 0){
			printf("there's some thing wrong\n");
		}
		else{
			printf("%s\n",buf);
		}

		/*n = read(sockfd, buf, MAXLINE);
		if (n == 0)
			printf("the other side has been closed.\n");
		else
			write(STDOUT_FILENO, buf, n);
			printf("%s\n", buf);*/
	}
	close(sockfd);
	return 0;
}
