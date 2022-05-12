/*
myserv.c 
Based on server.c, using pthread to realize multi-tasking.
Conducted by Ma Wenxi. Contributed by Zeng Junkai and Wang Yitong.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAXLINE 100
#define MAXCLIENTS 10
#define MAX_NAME_LENGTH 20
#define MIN_NAME_LENGTH 2
#define SERV_PORT 8000
#define ERROR_USR_DISCONNECTED 1
#define ERROR_USR_NAME_TOO_SHORT 2
#define ERROR_USR_NAME_TOO_LONG 3
#define ERROR_USR_NAME_CONFLICT 4
#define SUCCESS_LOGIN 0

struct sockaddr_in servaddr, cliaddr;
socklen_t cliaddr_len;
int listenfd, connfd;
char buf[MAXLINE]; // string buffer.
char str[INET_ADDRSTRLEN];
int client_fd_list[MAXCLIENTS]; //客户端的socketfd,10个元素，fds[0]~fds[9]
char usrnm_list[MAXCLIENTS][MAX_NAME_LENGTH]={0}; //initialize the username list.
int n; //n stores number of bytes read.
pid_t pid; 

int is_exit(char* buf)
{
	if(strcmp(buf,"exit!") == 0)
		return 1;
	else
		return 0;
}

/* send a message to all logged clients */
void SendMsgToAll(char* msg, int id){
    int i;
/* add "[username]" as prefix of the message. */
	char msgbuf[MAXLINE + MAX_NAME_LENGTH + 2] = "[";
	strcat(msgbuf, usrnm_list[id]);
	strcat(msgbuf, "]");
	strcat(msgbuf, msg);
	printf("0st %s\n",msgbuf);
    for (i = 0;i < MAXCLIENTS;i++){
        if (strlen(usrnm_list[i]) != 0){
            printf("sendto%d\n",client_fd_list[i]);
            send(client_fd_list[i],msgbuf,strlen(msgbuf),0);
        }
    }
}

/* server send a message to all logged clients */
void serverSendMsgToAll(char* msg){
    int i;
/* add "[username]" as prefix of the message. */
	char msgbuf[MAXLINE + MAX_NAME_LENGTH + 2] = "[server]";
	strcat(msgbuf, msg);
	printf("0st %s\n",msgbuf);
    for (i = 0;i < MAXCLIENTS;i++){
        if (strlen(usrnm_list[i]) != 0){
            printf("sendto%d\n",client_fd_list[i]);
            send(client_fd_list[i],msgbuf,strlen(msgbuf),0);
        }
    }
}

/* This function is now deprecated 
//regist a new client, input the pointer of clinet list, pointer of clinet name lenth and the pointer of clinet number
int regist(char **list,int *lenth, int *n, char *newname){
	int newlenth=0, signal=1;
	//save the lenth of new client name into "newname"
	for(int newlenth=0; newname[newlenth]!='\0'; newlenth++);
	//compare newname with the name in list
	for(int i=0; i<*n; i++){
		int sign=0;
		if(lenth[i] == newlenth)
			sign = 1;
		for(int j = 0; j < lenth[i]&&sign; j++){
			if(list[i][j] != newname[j]){
				sign = 0;
			}
		}
		if(sign)signal = 0;
	}
	if(signal){
		list[*n] = newname;
		lenth[*n] = newlenth;
		*n++;
	}
	return signal;
}
*/

int usrlogin (int id)
/* editted form Wang Yitong and Zeng Junkai's contributions */
{
	int user_count, min_slot, fd;
	int i, conflict_flag;
	char namebuf[MAXLINE]={};
	
	fd = client_fd_list[id];

/* When the client is naturally closed before logged in,
   just delete its file descriptor.
*/	
	if (recv(fd,namebuf,sizeof(namebuf),0) <= 0)
	{
		client_fd_list[id] = 0;
		printf("登录前退出：fd = %dquit\n",fd);
		pthread_exit((void*)id);
	}
	if (strlen(namebuf) >= MIN_NAME_LENGTH && strlen(namebuf) <= MAX_NAME_LENGTH) //这个条件我是后加的，如果先删除一个用户名再输入一个用户名会直接判定一次回车，不知道什么原因
	{
		conflict_flag = 0;
		for (i = 0; i < MAXCLIENTS; i++)
		{
			if (strlen(usrnm_list[i]))
				conflict_flag = !(strcmp(namebuf, usrnm_list[i]));
		}
		if (conflict_flag == 1)
		{
			return(ERROR_USR_NAME_CONFLICT);
		}
		else
		{
			strcpy(usrnm_list[id], namebuf);
			printf("username is %s\n", namebuf);
			return(SUCCESS_LOGIN);
		}
	}
	else if (strlen(namebuf) < MIN_NAME_LENGTH)
		return(ERROR_USR_NAME_TOO_SHORT);
	else 
		return(ERROR_USR_NAME_TOO_LONG);
}


/* function to run server initiation */
void init()
{
	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);
    
	bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

	listen(listenfd, 20);

	printf("Server started. Accepting connections...\n");
}

/* function to run single-client service. */
void* service_thread(void* p){
/* get the id and sockfd of this client */    
	int id = *(int*)p;
	int fd = client_fd_list[id];
    printf("pthread = %d\n",fd);
	int if_login = 0; //incicates if user is logged in.
	int log_stat = 0; //different cases of login status.
/* loop when not logged in */
	while (!if_login)
	{
		log_stat = usrlogin(id);
		char str[100];
		switch (log_stat)
		{
		case ERROR_USR_NAME_CONFLICT:
			strcpy(str, "Sorry, this username is being used. Please try another one.");
			send(fd,str,strlen(str),0); 
			break;
		case ERROR_USR_NAME_TOO_SHORT:
			strcpy(str, "Sorry, this username is too short. Please try a longer one.");
			send(fd,str,strlen(str),0); 
			break;
		case ERROR_USR_NAME_TOO_LONG:
			strcpy(str, "Sorry, this username is too long. Please try a shorter one.");
			send(fd,str,strlen(str),0); 
			break;
		case SUCCESS_LOGIN:
			strcpy(str, "You have successfully logged in.");
			send(fd,str,strlen(str),0);
			break;
		default:
			break;
		}
		if (log_stat == SUCCESS_LOGIN)
			if_login = 1;
	}
/* loop when successfully logged in */
    while(1){
        char buf[MAXLINE] = {0};
/* When the client is naturally closed,
   send a message to other clients and delete its file descriptor,
*/
        if (recv(fd,buf,sizeof(buf),0) <= 0 || is_exit(buf))
		{
			strcpy(buf, usrnm_list[id]);
			strcat(buf, " has left the chatroom");
			serverSendMsgToAll(buf);
			client_fd_list[id] = 0;
			for (int i = 0; i < MAX_NAME_LENGTH; i++)
				usrnm_list[id][i] = 0;
			printf("登录后退出：fd = %dquit\n",fd);
			pthread_exit((void*)id);
        }
        //把服务器接受到的信息发给所有的客户端
        else
		{
			printf("Received messsage\n");
			SendMsgToAll(buf, id);
		}
    }
}

/* function to run main service. */
void service_main()
{
	while (1) 
	{
		cliaddr_len = sizeof(cliaddr);
		connfd = accept(listenfd, 
				(struct sockaddr *)&cliaddr, &cliaddr_len);
		if (connfd == -1){
            printf("客户端连接出错...\n");
            continue;
        }
		int client_id = 0;
        for (client_id = 0;client_id < MAXCLIENTS;client_id++)
		{
            if (client_fd_list[client_id] == 0)
			{
                //记录客户端的socket
                client_fd_list[client_id] = connfd;
                printf("fd = %d\n",connfd);
                //有客户端连接之后，启动线程给此客户服务
                pthread_t tid;
                pthread_create(&tid,0,service_thread,&client_id);
                break;
        	}

			if (client_id == MAXCLIENTS)
			{
				//发送给客户端说聊天室满了
				char* str = "对不起，聊天室已经满了!";
				send(connfd,str,strlen(str),0); 
				close(connfd);
        	}
		}
	}
}

int main(void)
{	
	init(); // Call this function to initialize the server.
	service_main(); // Call this function to run main service.
}
