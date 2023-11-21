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
#include <stdio.h>
#include <errno.h>
#include <readline/readline.h>
#include <readline/history.h>


#define BUF_SIZE 65565

static char *line = (char *)NULL;

char *get_line() {
  int len;
  if (line) {
    free(line);
    line = (char *)NULL;
  }
  line = readline("\033[0;31mprompt> \033[0m");

  if (line && *line) {
    printf("akuku\n");
    add_history(line);
    len = strlen(line);
    line = realloc(line, len + 3);
    line[len] = '\r';
    line[len + 1] = '\n';
    line[len + 2] = '\0';
  }
  return line;
}

int create_socket(char* port, char* ip) {
    int sock; /* socket is a file descriptor */
    struct sockaddr_in addr; /* cast in bind on generic struct sockaddr */
    addr.sin_family = AF_INET;
    addr.sin_port = atoi(port);
    addr.sin_addr.s_addr = inet_addr(ip); /* 0.0.0.0 */
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        errx(EXIT_FAILURE, "Unable to create socket");
    return sock;
}


SSL_CTX *create_context() {
    const SSL_METHOD *method = TLS_client_method();
    SSL_CTX *ctx = SSL_CTX_new(method);
    if (ctx == NULL) {
        perror("Unable to create SSL context");
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    return ctx;
}
void configure_context(SSL_CTX *ctx, char* cert_name) {

    if (SSL_CTX_use_certificate_file(ctx, "client-cert.pem", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    if (SSL_CTX_use_PrivateKey_file(ctx, "client-key.pem", SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    if (SSL_CTX_load_verify_locations(ctx, "ca.pem", ".") <1) {
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
}

char *port, *host, *ca_name;
char buffer[BUF_SIZE];


#define PORT "7777"
#define IP "0.0.0.0"


int main(int argc, char *argv[]) {
    SSL_library_init();
	SSL_load_error_strings();
	OpenSSL_add_all_algorithms();

    if(argc <= 3)
    {
        errx(EXIT_FAILURE, "Not enough params");
    }

    port = argv[1];
    host = argv[2];
    ca_name = argv[3];
    printf("%s %s %s\n", host, port, ca_name);

    int sock; /* listening and connected socket */
    SSL_CTX *ctx;
    ctx = create_context();
    configure_context(ctx,ca_name);

    struct sockaddr_in addr; /* cast in bind on generic struct sockaddr */
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(atoi(port));
    addr.sin_addr.s_addr = inet_addr(host);

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        errx(EXIT_FAILURE, "Unable to create socket");
    
    pid_t pid;
    SSL *ssl;

    if ((connect(sock,(struct sockaddr*)&addr, sizeof(addr))) < 0)
    {
        printf("Socket returned error #%s,program terminated\n",strerror(errno));
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        exit(0);
        errx(EXIT_FAILURE, "Unable to connect");
    }

    ssl = SSL_new(ctx); 
    SSL_set_fd(ssl,sock);
        

    int err = SSL_connect(ssl);
    if (err < 1)
    {
        err=SSL_get_error(ssl,err);
        printf("SSL error #%d in accept,program terminated\n",err);

        if(err==5){printf("sockerrno is:%s\n",strerror(errno));}
        
        close(sock);
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        exit(0);
    }

    char* input;

    while(1)
    {
        input = get_line();
        if(*input != '\0')
        {
            printf("oto input: %s", input);
            SSL_write(ssl,input,strlen(input)+1);
            int bytes = SSL_read (ssl, buffer, sizeof(buffer));
            buffer[bytes] = '\0';
            printf("%s\n",buffer);
        } else 
        {
            break;
        }
    }

    SSL_shutdown(ssl); /* close SSL connection */
    SSL_free(ssl); /* recover some memory */
    close(sock);
    SSL_CTX_free(ctx);
}