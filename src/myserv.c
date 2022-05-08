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

int ifexit(char* x)
{
	char out[50] = "exit";
	int t = 1;
	for (int i = 0; x[i] != '\0' && t && i < 5; i++) {
		if (x[i] != out[i])t = 0;
	}
	return t;
}

int writeoff(char** list, int* lenth, int* n, char* delname)
{
	int dellenth = 0, signal = 0, num = -1;
	//save the lenth of delete client name into "delname"
	while (delname[dellenth] != '\0')
		dellenth++;
	//find the target name
	for (int i = 0; i < *n && !signal; i++)
	{
		if (lenth[i] == dellenth)
		{
			int sign = 1;
			for (int j = 0; j < lenth[i] && sign; j++)
			{
				if (list[i][j] != delname[j])
					sign = 0;
			}
			if (sign)
			{
				signal = 1;
				num = i;
			}
		}
	}
	//free the pointer and relist the list
	free(list[num]);
	(* n)--;
	for (int i = num; i < *n; i++) {
		list[i] = list[i + 1];
		lenth[i] = lenth[i + 1];
	}
	return signal;
}


//regist a new client, input the pointer of clinet list, pointer of clinet name lenth and the pointer of clinet number
int regist(char** list, int* lenth, int* n, char* newname) {
	int newlenth = 0, signal = 1;
	char* newname1;
	//save the lenth of new client name into "newname"
	while (newname[newlenth] != '\0')
		newlenth++;
	newname1 = (char*)malloc((newlenth + 1) * sizeof(char));
	//copy the string in newname to newname1
	for (int i = 0; i <= newlenth; i++) 
	{
		newname1[i] = newname[i];
	}
	//compare newname with the name in list
	for (int i = 0; i < *n; i++) {
		int sign = 0;
		if (lenth[i] == newlenth)sign = 1;
		for (int j = 0; j < lenth[i] && sign; j++) {
			if (list[i][j] != newname1[j])
			{
				sign = 0;
			}
		}
		if (sign)signal = 0;
	}
	//if there is no same name, add the newname into the list
	if (signal)
	{
		list[*n] = newname1;
		lenth[*n] = newlenth;
		(*n)++;
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
