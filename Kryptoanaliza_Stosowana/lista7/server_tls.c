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


SSL_CTX *create_context() {
    const SSL_METHOD *method = TLS_server_method();
    SSL_CTX *ctx = SSL_CTX_new(method);
    if (ctx == NULL) {
        perror("Unable to create SSL context");
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    return ctx;
}
void configure_context(SSL_CTX *ctx) {


    if (SSL_CTX_use_certificate_file(ctx, "server-cert.pem", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    if (SSL_CTX_use_PrivateKey_file(ctx, "server-key.pem", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    if (SSL_CTX_load_verify_locations(ctx,"ca.pem", ".") <1) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    SSL_CTX_set_verify(ctx,SSL_VERIFY_PEER|SSL_VERIFY_FAIL_IF_NO_PEER_CERT,NULL);
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

void handle_client(int client, SSL *ssl)
{
    while(1)
    {
        char buffer[BUF_SIZE];
        
        size_t size = SSL_read(ssl, buffer, sizeof(buffer));

        buffer[size] = '\0';

        if(size > 2)
        {  
            printf("Recieved: %s of size %ld\n", buffer, size);
            reverse_buffer(buffer, size);
            printf("After Reverse: %s\n", buffer);
            SSL_write(ssl, buffer, size);
        } else 
        {
            printf("Client Died\n");
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    SSL_library_init();
    SSL_load_error_strings();

    int sock, client; /* listening and connected socket */
    SSL_CTX *ctx;
    ctx = create_context();
    configure_context(ctx);
    sock = create_socket(PORT);
    while (1) {
        struct sockaddr_in addr;
        socklen_t len = sizeof(addr);
        pid_t pid;
        SSL *ssl;
        if ((client = accept(sock, (struct sockaddr*)&addr, &len)) < 0)
            errx(EXIT_FAILURE, "Unable to accept");
        
        printf("accepted new client!\n");

        if((pid = fork()) < 0)
        {
            errx(EXIT_FAILURE, "Fork Error");
        }
        else if(pid == 0)
        {

            ssl = SSL_new(ctx); /* create new SSL structure */
            SSL_set_fd(ssl, client); /* wrap connected socket with SSL */
            if (SSL_accept(ssl) <= 0)
            {
                printf("nie ma ac ;c\n");
                ERR_print_errors_fp(stderr); /* if screwed up, diagnose the failure */
            } else
            {
                // if (SSL_get_verify_result(ssl) != X509_V_OK) {
                //     printf("SSL Client Authentication error\n");
                //     SSL_free(ssl);
                //     close(client);
                //     SSL_CTX_free(ctx);
                //     exit(0);
                // }
                handle_client(client, ssl);
            }

            SSL_shutdown(ssl); /* close SSL connection */
            SSL_free(ssl); /* recover some memory */
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
    SSL_CTX_free(ctx);
}