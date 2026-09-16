#include "include/headers.h"
#include "include/timestamp.h"
#include "include/handle_client.h"
#include "include/handle_client_functions.h"

int handle_client_interaction(int client_sockfd, FILE *server_log, client_session_id_t *client_session)
{
    char recv_buffer[1024];
    char line_buffer[4096];
    size_t line_len = 0;

    while (1)
    {
        ssize_t n = recv(client_sockfd, recv_buffer, sizeof(recv_buffer), 0);

        if (n == 0)
        {
            fprintf(server_log, "[%s] [WARN] (DISCONNECTED) IP='%s' PORT='%d' ID='%d'\n", get_timestamp(), client_session->ip, client_session->port, client_session->id);
            break;
        }

        if (n < 0)
        {
            fprintf(server_log, "[%s] [ERROR] (RECV) IP='%s' PORT='%d' ID='%d' ERROR='%s'\n", get_timestamp(), client_session->ip, client_session->port, client_session->id, strerror(errno));
            break;
        }

        for (ssize_t i = 0; i < n; i++)
        {
            char c = recv_buffer[i];

            line_buffer[line_len++] = c;

            if (c == '\n')
            {
                line_buffer[line_len - 1] = '\0';

                line_len = 0;
            }
        }

    }
}