#include "include/headers.h"
#include "include/client_session_t.h"
#include "include/handle_client_functions.h"
#include "include/timestamp.h"


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
}

void get_metadata(char *command, FILE *log, client_session_id_t *session)
{
    char *tokens[10];
    int ntokens = 0;

    char *token = strtok(command, " ");

    while (token != NULL && ntokens < 10)
    {
        tokens[ntokens++] = token;
        token = strtok(NULL, " ");
    }

    fprintf(log, "[%s] [INFO] (FILE RECEIVED) IP='%s' ID='%d' FILE_NAME='%s' FILE_SIZE='%s'\n", get_timestamp(), session->ip, session->id, tokens[1], tokens[2]);
}