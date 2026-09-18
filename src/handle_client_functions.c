#include "../include/headers.h"
#include "../include/client_session_t.h"
#include "../include/handle_client_functions.h"
#include "../include/timestamp.h"

command_state_t parse_command(char *command)
{
    char *tokens[10];
    int ntokens = 0;

    char *token = strtok(command, " ");

    while (token != NULL && ntokens < 10)
    {
        tokens[ntokens++] = token;
        token = strtok(NULL, " ");
    }

    if (ntokens == 3)
    {
        if (strcmp("METADATA", tokens[0]) == 0)
        {
            return METADATA;
        }
    }

    return UNKNOWN;
}


int parse_metadata(char *metadata, char *file_name, size_t file_name_size, size_t *file_size)
{
    char command[32];

    unsigned long size;

    int result = sscanf(metadata, "%31s %255s %lu", command, file_name, &size);

    if (result != 3)
    {
        return -1;
    }

    if (strcmp(command, "METADATA") != 0)
    {
        return -1;
    }

    if (strlen(file_name) >= file_name_size)
    {
        return -1;
    }

    *file_size = (size_t)size;

    return 0;
}


int recv_line(int client_sockfd, char *buffer, size_t buffer_size)
{
    size_t i = 0;

    while (i < buffer_size - 1)
    {
        char c;

        ssize_t n = recv(client_sockfd, &c, 1, 0);

        if (n == 0)
        {
            return 0;
        }

        if (n < 0)
        {
            return -1;
        }

        if (c == '\n')
        {
            buffer[i] = '\0';

            return 1;
        }

        buffer[i++] = c;
    }

    buffer[0] = '\0';

    return -2;
}


int receive_file(int client_sockfd, const char *file_name, size_t file_size)
{
    char file_path[252];

    snprintf(file_path, sizeof(file_path), "tmp/%s", file_name);

    FILE *file = fopen(file_path, "wb");

    if (file == NULL)
    {
        return -1;
    }

    char buffer[8192];

    size_t total_received = 0;

    while (total_received < file_size)
    {
        size_t remaining = file_size - total_received;

        size_t bytes_to_receive;

        if (remaining < sizeof(buffer))
        {
            bytes_to_receive = remaining;
        }
        else
        {
            bytes_to_receive = sizeof(buffer);
        }

        ssize_t n = recv(client_sockfd, buffer, bytes_to_receive, 0);

        if (n == 0)
        {
            fclose(file);

            return -2;
        }

        if (n < 0)
        {
            fclose(file);

            return -3;
        }

        size_t written = fwrite(buffer, 1, n, file);

        if (written != (size_t)n)
        {
            fclose(file);

            return -4;
        }

        total_received += n;
    }

    fclose(file);

    return 0;
}


int upload_file_s3(const char *file_path)
{
    char command[252];

    snprintf(command, sizeof(command), "aws s3 cp '%s' 's3://ruizsocket-server-files/'", file_path);

    return system(command);
}