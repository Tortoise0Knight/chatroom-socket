/* server.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXLINE 80
#define MAXCLIENTS 100
#define SERV_PORT 8000

//regist a new client, input the pointer of clinet list, pointer of clinet name lenth and the pointer of clinet number
int regist(char **list,int *lenth; int *n; char *newname){
	int newlenth=0, signal=1;
	//save the lenth of new client name into "newname"
	for(int newlenth=0; newname[newlenth]!='\0'; newlenth++);
	//compare newname with the name in list
	for(int i=0; i<*n; i++){
		int sign=0;
		if(lenth[i]==newlenth)sign=1;
		for(int j=0; j<lenth[i]&&sign; j++){
			if(char[i][j]=!=newname[j]){
				sign=0;
			}
		}
		if(sign)signal=0;
	}
	if(signal){
		list[*n]=newname;
		lenth[*n]=newlenth;
		*n++;
	}
	return signal;
}

int main(void)
{
	struct sockaddr_in servaddr, cliaddr;
	socklen_t cliaddr_len;
	int listenfd, connfd;
	char buf[MAXLINE];
	char str[INET_ADDRSTRLEN];
	int i, n;
	pid_t pid;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);
    
	bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	listen(listenfd, 20);

	printf("Server started. Accepting connections...\n");
	while (1) {
		cliaddr_len = sizeof(cliaddr);
		connfd = accept(listenfd, 
				(struct sockaddr *)&cliaddr, &cliaddr_len);
		pid = fork();
/* Handle if creating new process unsuccessful */
		if (pid == -1)
		{
			printf("Fork error. Program exited.\n!");
            exit(1);
		}
/* Child proccess to handle a certain client request */
		else if (pid == 0)
		{
			close(listenfd);
			printf("Connection established with %s.\n", 
				inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)));
			while (1) {
				n = read(connfd, buf, MAXLINE);
				if (n == 0) {
					printf("the other side has been closed.\n");
					break;
				}
				printf("received from %s at PORT %d\n",
					inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),
					ntohs(cliaddr.sin_port));
		
				for (i = 0; i < n; i++)
					buf[i] = toupper(buf[i]);
				write(connfd, buf, n);
			}
			close(connfd);
			exit(0);
		}
/* Parent proccess to prepare for another connection */
		else
		{
			close(connfd);
		}
		
	}
}
