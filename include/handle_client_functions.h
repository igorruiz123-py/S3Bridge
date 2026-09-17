typedef enum
{
    METADATA,
    UNKNOWN

} command_state_t;

command_state_t parse_command(char *command);

void get_metadata(char *command, FILE *log, client_session_id_t *session);