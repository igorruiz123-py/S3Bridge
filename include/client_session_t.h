#include "include/headers.h"

typedef struct
{
    char ip[INET_ADDRSTRLEN];
    int id;
    int port;

} client_session_id_t;