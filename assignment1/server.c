// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 8080
#define NOBODY_ID -2 // On MacOS user id for nobody is -2
int main(int argc, char const *argv[]) 
{ 
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0}; 
    char *hello = "Hello from server"; 
       
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 8080
    // https://stackoverflow.com/questions/58599070/socket-programming-setsockopt-protocol-not-available#comment103510725_58599070
    // https://stackoverflow.com/a/14388707
    if (
        setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) ||
        setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt))
    ) {
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) 
    { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    }
    int pid = fork();
    if (pid == -1)
    {
        perror("creation of a child process was unsuccessful");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        printf("inside child process\n");
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
    }
    else if (pid > 0)
    {
        printf("inside parent process\nwaiting for child to finish\n");
        int child_status;
        waitpid(pid, &child_status, 0);
        if (child_status > 0)
        {
            perror("child process failed with error");
            exit(EXIT_FAILURE);
        }
        else
        {
            printf("child processed connection from client\n");
        }
    } 
    return 0; 
} 
