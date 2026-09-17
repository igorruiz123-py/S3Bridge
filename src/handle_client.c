#include "include/client_session_t.h"
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
            fprintf(server_log, "[%s] [WARN] (DISCONNECTED) IP='%s' ID='%d'\n", get_timestamp(), client_session->ip, client_session->id);
            break;
        }

        if (n < 0)
        {
            fprintf(server_log, "[%s] [ERROR] (RECV) IP='%s' ID='%d' ERROR='%s'\n", get_timestamp(), client_session->ip, client_session->id, strerror(errno));
            break;
        }

        for (ssize_t i = 0; i < n; i++)
        {
            char c = recv_buffer[i];

            line_buffer[line_len++] = c;

            if (c == '\n')
            {
                line_buffer[line_len - 1] = '\0';

                char metadata_buffer[4096];

                char command_buffer[4096];

                strcpy(command_buffer, line_buffer);

                strcpy(metadata_buffer, line_buffer);

                command_state_t parse_status = parse_command(command_buffer);

                if (parse_status == METADATA)
                {
                    get_metadata(metadata_buffer, server_log, client_session);
                }

                line_len = 0;
            }
        }

    }
}