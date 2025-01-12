#ifndef SERVER_H
#define SERVER_H

#define _XOPEN_SOURCE   600
#define _POSIX_C_SOURCE 200112L

#include "ipc.h"

void place_fruit(GameState *state);
int check_wall_collision(GameState *state);
int check_self_collision(GameState *state);
void update_snake_body(GameState *state);
void move_snake(GameState *state);
void init_server();

#endif

