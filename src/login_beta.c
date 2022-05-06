#include <stdio.h>
#include <string.h>


#define MAX_USER 5		//聊天室内最大人数
#define MAX_CHAR 10		//用户名最大字符数

int main()
{
	char usrnm[MAX_CHAR];
	char usrnm_list[MAX_USER][MAX_CHAR] = { 0 };		//用户名列表，第一个角标相当于id
	int user_count, min_slot;
	int i, flag, k;
	char mode;
	int id;

	user_count = 0;		//当前在聊天室内的用户数
	min_slot = -1;		//用户名列表中的最小空位序号
	while (1)
	{
		printf("Choose mode:\n\
			1: Enter username\n\
			2: Delete username\n\
			3: Show username list\n");
		while (1)
		{
			mode = _getch();
			if (mode == '1' || mode == '2' || mode == '3')
				break;
		}
		if (mode == '1')
		{
			printf("Enter your username: ");
			gets(usrnm);
			if (strlen(usrnm))
				//这个条件我是后加的，如果先删除一个用户名再输入一个用户名会直接判定一次回车，不知道什么原因
			{
				if (user_count == MAX_USER)
				{
					printf("Sorry, the username list is full.\n");
					continue;
				}
				flag = 1;
				k = 0;
				for (i = 0; i < MAX_USER; i++)
				{
					if (strlen(usrnm_list[i]))
						flag = strcmp(usrnm, usrnm_list[i]);
					else
					{
						if (k == 0)
						{
							min_slot = i;
							k = 1;
						}
					}
					if (flag == 0)
					{
						printf("Username already exists.\n");
						break;
					}
				}
				if (flag != 0)
				{
					if (k == 0)
						min_slot = i;
					strcpy(usrnm_list[min_slot], usrnm);
					user_count++;
				}
			}
			else
				printf("The username could not be empty.\n");
		}
		if (mode == '2')
		{
			printf("Enter the ID of the target username: ");
			scanf("%d", &id);
			if (id >= 0 && id < MAX_USER)
			{
				if (strlen(usrnm_list[id]))
				{
					for (i = 0; i < MAX_CHAR; i++)
						usrnm_list[id][i] = 0;
					user_count--;
				}
				else
					printf("User does not exist.\n");
			}
			else
				printf("Invalid ID entry.\n");
		}
		if (mode == '3')
		{
			printf("\n------Username List------\n");
			for (i = 0; i < MAX_USER; i++)
			{
				if (strlen(usrnm_list[i]))
				{
					printf("%d ", i);
					puts(usrnm_list[i]);
				}
			}
			printf("\n");
		}
	}
	return 0;
}
