#include <stdio.h>
#include <sys/select.h>

int main()  
{  
    fd_set fdset;  
    int isset;

    FD_ZERO(&fdset);  
    FD_SET(3, &fdset);  
    FD_SET(5, &fdset);  

    isset = FD_ISSET(3, &fdset);  
    printf("isset = %d\n", isset);  

    FD_CLR(3, &fdset);  
    isset = FD_ISSET(3, &fdset);  
    printf("isset = %d\n", isset);  
    isset = FD_ISSET(5, &fdset);  
    printf("isset = %d\n", isset);  

    return 0;  
}  
