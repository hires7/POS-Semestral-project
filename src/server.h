#ifndef SERVER_H
#define SERVER_H

#define _XOPEN_SOURCE   600
#define _POSIX_C_SOURCE 200112L

#include "ipc.h"

// Deklarácia funkcií
void init_server();
void place_fruit(GameState *state);

#endif

