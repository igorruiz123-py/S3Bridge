#include "include/headers.h"
#include "include/server_functions.h"
#include "include/timestamp.h"

int create_server_socket(FILE *log)
{
    int sockfd;
    struct addrinfo hints, *serverinfo, *p;
    int yes=1;
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((rv = getaddrinfo(NULL, PORT, &hints, &serverinfo)) != 0) 
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    for(p = serverinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) 
        {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) 
        {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) 
        {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(serverinfo);

    if (p == NULL)  
    {
        fprintf(stderr, "server: failed to bind\n");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_storage addr;
    socklen_t len = sizeof(addr);

    getsockname(sockfd, (struct sockaddr *)&addr, &len);

    char ip[INET6_ADDRSTRLEN];

    if (addr.ss_family == AF_INET)
    {
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)&addr;

        inet_ntop(AF_INET, &ipv4->sin_addr, ip, sizeof(ip));
    }

    fprintf(log, "[%s] [INFO] Server initialized.\n", get_timestamp());
    fprintf(log, "[%s] [INFO] Server listening on %s:%s\n", get_timestamp(), ip, PORT);

    if (listen(sockfd, BACKLOG) == -1) 
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    return sockfd;
}

