#include <stdio.h> // printf
#include <sys/socket.h> // socket
#include <arpa/inet.h> // htonl
#include <string.h> // strdup
#include <stdlib.h> // exit
#include <unistd.h> // close
#include <pthread.h> // pthread

#define PORT 8085
int sd = -1, client_sockfd = -1;

void * handle_request(void * arg)
{
    int client = *(int *)arg; // just copy, so client_sockfd can be reused
    printf("client: %d \n", client);
    char buf[1024];
    sprintf(buf, "HTTP/1.1 200 OK\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "Content-type: text/html\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "<p>Welcome to my world!</p>\r\n");
    send(client, buf, strlen(buf), 0);
//    sleep(10); // test for blocking
    close(client); // should close it just here
    return NULL;

}
int main(int argv, char *argc[])
{
    pthread_t tid;
    
    int port = PORT;
    if(argv > 1)
    {
        port = atoi(argc[1]);
    }

    struct sockaddr_in addr; // addr of server
    bzero(&addr, sizeof(addr)); // memset is also ok
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
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
    printf("Listening to port: %d \n", port);
    while(1)
    {
        if((client_sockfd = accept(sd, NULL, NULL))<0)
        {
            perror("accept error");
            exit(1);
        }
        char *msg = strdup("hello\n");
        printf("%s", msg);
        if(pthread_create(&tid, NULL, handle_request, (void *)&client_sockfd)!=0)
        {
            perror("thread creation failed");
            continue;
        }
        printf("world!\n");
        //close(client_sockfd); // terrible mistake
    }

    close(sd);
    return 0;
}
