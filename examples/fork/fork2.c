#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int 
main()
{
	int num = 0;
	int *p =&num;

	pid_t		pid;
	pid = fork();


	while (1) {
        printf("Hello, World!\n");

		if (pid == 0)
		{
			printf("Child process! The number is %d\n", *p);
			sleep(3);
		}
		else
		{
			printf("Parent process! The number is %d\n", *p);
			sleep(3);
			(*p)++;
		}
	}
	return 0;
}
