// Program which will be executed by child process
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define NOBODY_ID -2 // On MacOS user id for nobody is -2

int main(int argc, char const *argv[])
{
    int new_socket = atoi(argv[0]);
    int valread;
    char buffer[1024] = {0};
    char *hello = "Hello from server";
    if (setuid(NOBODY_ID) < 0)
    {
        perror("setuid failure");
        exit(EXIT_FAILURE);
    }
    else
    {
        valread = read( new_socket , buffer, 1024);
        printf("%s\n", buffer);
        send(new_socket , hello , strlen(hello) , 0 );
        printf("Hello message sent\n");
    }
    return 0;
}
