#include "include/timestamp.h"

char* get_timestamp() {
    static char buffer[30];
    time_t now;
    struct tm *local;

    time(&now);
    local = localtime(&now);

    strftime(buffer, sizeof(buffer), "%d/%m/%Y - %H:%M:%S", local);

    return buffer;
}