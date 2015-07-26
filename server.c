#include <stdio.h> // printf
#include <sys/socket.h> // socket
#include <arpa/inet.h> // htonl
#include <string.h> // strdup
#include <stdlib.h> // exit
#include <unistd.h> // close

#define PORT 8085
int sd = -1, client_sockfd = -1;

void handle_request(int client)
{
    char buf[1024];
    sprintf(buf, "HTTP/1.1 200 OK\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "Content-type: text/html\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "<p>Welcome to my world!</p>\r\n");
    send(client, buf, strlen(buf), 0);

}
int main(int argv, char *argc[])
{
    struct sockaddr_in addr; // addr of server
    bzero(&addr, sizeof(addr)); // memset is also ok
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY); // any ipv4 available
    if((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) // attention: bracket
    {
        perror("socket creation failed!");
        exit(1);
    }
    if(bind(sd, (struct sockaddr*)&addr, sizeof(addr))<0)
    {
        perror("bind failed");
        exit(1);
    }
    if(listen(sd, 10)<0)
    {
        perror("listen failed");
        exit(1);
    }
    printf("Listening to port: %d \n", PORT);
    while(1)
    {
        if((client_sockfd = accept(sd, NULL, NULL))<0)
        {
            perror("accept error");
            exit(1);
        }
        char *msg = strdup("hello\n");
        printf("%s", msg);
        handle_request(client_sockfd);
        printf("world!\n");
        close(client_sockfd);
    }

    close(sd);
    return 0;
}
