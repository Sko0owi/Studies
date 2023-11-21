#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define PORT 7777
#define BUF_SIZE 65565


int create_socket(int port) {
    int sock; /* socket is a file descriptor */
    struct sockaddr_in addr; /* cast in bind on generic struct sockaddr */
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY); /* 0.0.0.0 */
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        errx(EXIT_FAILURE, "Unable to create socket");
    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0)
        errx(EXIT_FAILURE, "Unable to bind");
    if (listen(sock, 1) < 0)
        errx(EXIT_FAILURE, "Unable to listen");
    return sock;
}



void reverse_buffer(char* buffer, ssize_t size)
{
    ssize_t l = 0, r = size - 3;
    buffer[size-1] = '\n';
    buffer[size-2] = '\r';
    while(l <= r)
    {
        char tmp = buffer[l];

        buffer[l] = buffer[r];
        buffer[r] = tmp;
        
        l++; r--;
    }
}

void handle_client(int client)
{
    while(1)
    {
        char buffer[BUF_SIZE];
        size_t size = recv(client, buffer, sizeof(buffer), 0);

        if(size > 2)
        {  
            printf("Recieved: %s of size %ld\n", buffer, size);
            reverse_buffer(buffer, size);
            printf("After Reverse: %s\n", buffer);
            send(client, buffer, size, 0); 
        } else 
        {
            printf("Client Died\n");
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    int sock, client; /* listening and connected socket */
    sock = create_socket(PORT);
    while (1) {
        struct sockaddr_in addr;
        socklen_t len = sizeof(addr);
        pid_t pid;

        if ((client = accept(sock, (struct sockaddr*)&addr, &len)) < 0)
            errx(EXIT_FAILURE, "Unable to accept");
        
        if((pid = fork()) < 0)
        {
            errx(EXIT_FAILURE, "Fork Error");
        }
        else if(pid == 0)
        {
            handle_client(client);

            close(client);
            exit(EXIT_SUCCESS);
        } else 
        {
            close(client);

            int status;
			waitpid(pid, &status, WNOHANG);
        }
    }
    close(sock);
}