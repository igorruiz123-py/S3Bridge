typedef enum
{
    METADATA,
    UNKNOWN

} command_state_t;

command_state_t parse_command(char *command);

void get_metadata(char *command, FILE *log, client_session_id_t *session);

int parse_metadata(char *metadata, char *file_name, size_t file_name_size, size_t *file_size);

int recv_line(int client_sockfd, char *buffer, size_t buffer_size);

int receive_file(int client_sockfd, const char *file_name, size_t file_size);

int upload_file_s3(const char *file_path);

