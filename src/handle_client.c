#include "../include/client_session_t.h"
#include "../include/headers.h"
#include "../include/timestamp.h"
#include "../include/handle_client.h"
#include "../include/handle_client_functions.h"


int handle_client_interaction(int client_sockfd, FILE *server_log, client_session_id_t *client_session)
{
    char metadata_buffer[4096];

    while (1)
    {
        int line_result = recv_line(client_sockfd, metadata_buffer, sizeof(metadata_buffer));

        if (line_result == 0)
        {
            fprintf(server_log, "[%s] [WARN] (DISCONNECTED) IP='%s' ID='%d'\n", get_timestamp(), client_session->ip, client_session->id);

            break;
        }

        if (line_result == -1)
        {
            fprintf(server_log, "[%s] [ERROR] (RECV) IP='%s' ID='%d' ERROR='%s'\n", get_timestamp(), client_session->ip, client_session->id,strerror(errno));

            break;
        }

        if (line_result == -2)
        {
            fprintf(server_log, "[%s] [ERROR] (METADATA_TOO_LONG) IP='%s' ID='%d'\n", get_timestamp(), client_session->ip, client_session->id);

            break;
        }

        char command_buffer[4096];

        strcpy(command_buffer, metadata_buffer);

        command_state_t parse_status = parse_command(command_buffer);

        if (parse_status != METADATA)
        {
            fprintf(server_log, "[%s] [WARN] (INVALID_COMMAND) IP='%s' ID='%d' DATA='%s'\n", get_timestamp(), client_session->ip, client_session->id, metadata_buffer);

            continue;
        }

        char file_name[256];

        size_t file_size;

        int metadata_result = parse_metadata(metadata_buffer, file_name, sizeof(file_name), &file_size);

        if (metadata_result < 0)
        {
            fprintf(server_log, "[%s] [ERROR] (INVALID_METADATA) IP='%s' ID='%d'\n", get_timestamp(), client_session->ip, client_session->id);

            continue;
        }

        fprintf(server_log, "[%s] [INFO] (FILE_RECEIVING) IP='%s' ID='%d' FILE_NAME='%s' FILE_SIZE='%zu'\n", get_timestamp(), client_session->ip, client_session->id, file_name, file_size);

        int receive_result = receive_file(client_sockfd, file_name, file_size);

        if (receive_result == 0)
        {
            fprintf(server_log, "[%s] [INFO] (FILE_RECEIVED) IP='%s' ID='%d' FILE_NAME='%s' FILE_SIZE='%zu'\n", get_timestamp(), client_session->ip, client_session->id, file_name, file_size);
        }

        else if (receive_result == -2)
        {
            fprintf(server_log, "[%s] [ERROR] (FILE_INCOMPLETE) IP='%s' ID='%d' FILE_NAME='%s' FILE_SIZE='%zu'\n", get_timestamp(), client_session->ip, client_session->id, file_name, file_size);

            break;
        }

        else
        {
            fprintf(server_log, "[%s] [ERROR] (FILE_RECEIVE) IP='%s' ID='%d' FILE_NAME='%s' ERROR='%s'\n", get_timestamp(), client_session->ip, client_session->id, file_name, strerror(errno));

            break;
        }
    }

    return 0;
}