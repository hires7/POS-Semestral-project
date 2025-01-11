#ifndef CLIENT_H
#define CLIENT_H

#include "ipc.h"

void *handle_input(void *arg);
void *receive_and_render(void *arg);
void init_client();

#endif

