#include "../include/headers.h"
#include "../include/server_functions.h"
#include "../include/timestamp.h"
#include "../include/client_session_t.h"
#include "../include/handle_client.h"

int main(void)
{
    srand(time(NULL));

    FILE *log = fopen(LOG_PATH, "a");

    if (log == NULL)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    setvbuf(log, NULL, _IONBF, 0);

    struct sockaddr_storage their_addr;
    socklen_t sin_size = sizeof(their_addr);
    char ip[INET_ADDRSTRLEN];

    int sockfd = create_server_socket(log);

     while (1)
    {
        sin_size = sizeof(their_addr);

        int client_sockfd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);

        if (their_addr.ss_family == AF_INET)
        {
            struct sockaddr_in *client;

            client = (struct sockaddr_in *)&their_addr;

            int client_port = ntohs(client->sin_port);

            inet_ntop(AF_INET, &client->sin_addr, ip, sizeof(ip));

            client_session_id_t client_session;
            memset(&client_session, 0, sizeof(client_session));
            client_session.port = client_port;
            strcpy(client_session.ip, ip);
            client_session.id = rand();

            fprintf(log, "[%s] [INFO] Client id=%d connected from %s:%d\n", get_timestamp(), client_session.id, ip, client_port);

            pid_t child = fork();

            if (child < 0)
            {
                perror("fork");
                exit(EXIT_FAILURE);
            }

            if (child == 0)
            {
                int status = handle_client_interaction(client_sockfd, log, &client_session);

                close(client_sockfd);

                exit(status);
            }

            close(client_sockfd);

        }
    }

    return 0;
}