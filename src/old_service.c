void service()
{
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
				logstat = 0;
			while (1) {
				n = read(connfd, buf, MAXLINE); // n returns the number of bytes read.
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
/* Parent proccess to prepare for another connection. */
		else
		{
			close(connfd);
		}
		
	}
}