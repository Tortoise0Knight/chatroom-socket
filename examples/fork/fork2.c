#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int 
main()
{
	pid_t		pid;
	pid = fork();


	while (1) {
        printf("Hello, World!\n");

		if (pid == 0)
			printf("Child process!\n");
		else
			printf("Parent process!\n");
	}
	return 0;
}
