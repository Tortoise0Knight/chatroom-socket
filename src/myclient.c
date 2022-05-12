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

void cleanout(char* buf);

char sendbuf[MAXLINE];
char readbuf[MAXLINE];
struct sockaddr_in servaddr;
char servaddrstr[INET_ADDRSTRLEN];
int sockfd, n, a;

int is_exit()
{
	fgets(sendbuf, MAXLINE, stdin);
	sendbuf[strcspn(sendbuf, "\n")] = 0;
	if(strcmp(sendbuf,"exit!") == 0)
		return 1;
	else
		return 0;
}

void* sendmesg(void* p)
{
	int sockfd = *(int*)p;
	while(1)
	{
		if (is_exit())
		{
			send(sockfd, sendbuf, strlen(sendbuf),0);
			close(sockfd);
			*(int*)p = 0;
			pthread_exit((void*)p);
		}
		else
		{
			send(sockfd, sendbuf, strlen(sendbuf),0);
			cleanout(sendbuf);
		}
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
	pthread_t tid;
    pthread_create(&tid,0,sendmesg,&sockfd);
	while(sockfd != 0)
	{
		cleanout(readbuf);
		if (recv(sockfd,readbuf,sizeof(readbuf),0) <= 0)
		{
			printf("There's something wrong with connection.\n");
			close(sockfd);
			sockfd = 0;
		}
		else
			printf("%s\n",readbuf);
	}
	return 0;
}
