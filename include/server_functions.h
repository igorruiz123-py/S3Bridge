#define LOG_PATH "logs/server.log"
#define BACKLOG 5
#define PORT "8080"

int create_server_socket(const char *port, FILE *server_log);