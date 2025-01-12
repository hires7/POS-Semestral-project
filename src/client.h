#ifndef CLIENT_H
#define CLIENT_H

#include "ipc.h"

#define _XOPEN_SOURCE   600
#define _POSIX_C_SOURCE 200112L

void *handle_input(void *arg);
void *receive_and_render(void *arg);
void init_client();

#endif
