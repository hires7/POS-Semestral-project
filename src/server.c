#include "server.h"
#include <unistd.h>
#include "ipc.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define MOVE_INTERVAL 500000

void place_fruit(GameState *state) {
    state->fruit_x = rand() % MAP_WIDTH;
    state->fruit_y = rand() % MAP_HEIGHT;
    printf("[Server] Ovocie umiestnené na (%d, %d)\n", state->fruit_x, state->fruit_y);
}

int check_wall_collision(GameState *state) {
    return state->snake_x < 0 || state->snake_x >= MAP_WIDTH ||
           state->snake_y < 0 || state->snake_y >= MAP_HEIGHT;
}

int check_self_collision(GameState *state) {
    for (int i = 0; i < state->snake_length; i++) {
        if (state->snake_x == state->snake_body[i][0] &&
            state->snake_y == state->snake_body[i][1]) {
            return 1;
        }
    }
    return 0;
}

void update_snake_body(GameState *state) {
    for (int i = state->snake_length - 1; i > 0; i--) {
        state->snake_body[i][0] = state->snake_body[i - 1][0];
        state->snake_body[i][1] = state->snake_body[i - 1][1];
    }
    state->snake_body[0][0] = state->snake_x;
    state->snake_body[0][1] = state->snake_y;
}

void move_snake(GameState *state) {
    switch (state->current_direction) {
        case CMD_MOVE_UP:    state->snake_y--; break;
        case CMD_MOVE_DOWN:  state->snake_y++; break;
        case CMD_MOVE_LEFT:  state->snake_x--; break;
        case CMD_MOVE_RIGHT: state->snake_x++; break;
        default: break;
    }
}

void init_server() {
    srand(time(NULL));

    printf("[Server] Inicializujem FIFO kanály...\n");
    pipe_destroy(FIFO_CLIENT_TO_SERVER);
    pipe_destroy(FIFO_SERVER_TO_CLIENT);

    pipe_init(FIFO_CLIENT_TO_SERVER);
    pipe_init(FIFO_SERVER_TO_CLIENT);

    FILE *ready = fopen("/tmp/server_ready", "w");
    if (!ready) {
        perror("[Server] Chyba pri vytváraní kontrolného súboru");
        exit(EXIT_FAILURE);
    }
    fclose(ready);

    printf("[Server] FIFO kanály pripravené. Čakám na príkazy...\n");

    int fd_read = pipe_open_read(FIFO_CLIENT_TO_SERVER);
    int fd_write = pipe_open_write(FIFO_SERVER_TO_CLIENT);

    GameState state = {5, 5, 1, {{5, 5}}, 0, 0, 0, 1, CMD_MOVE_RIGHT};
    place_fruit(&state);

    Command cmd;
    int is_paused = 0;

    while (state.is_running) {
        if (is_paused) {
            pipe_read(fd_read, &cmd, sizeof(Command));
            if (cmd == CMD_PAUSE) {
                printf("[Server] Pauza ukončená.\n");
                is_paused = 0;
            }
            continue;
        }

        fd_set fds;
        struct timeval timeout = {0, 0};

        FD_ZERO(&fds);
        FD_SET(fd_read, &fds);

        int ready = select(fd_read + 1, &fds, NULL, NULL, &timeout);

        if (ready > 0) {
            pipe_read(fd_read, &cmd, sizeof(Command));
            if (cmd == CMD_PAUSE) {
                printf("[Server] Hra pozastavená.\n");
                is_paused = 1;
                continue;
            } else if (cmd == CMD_EXIT) {
                state.is_running = 0;
            } else if (cmd >= CMD_MOVE_UP && cmd <= CMD_MOVE_RIGHT) {
                state.current_direction = cmd;
            }
        }

        move_snake(&state);

        if (check_wall_collision(&state) || check_self_collision(&state)) {
            printf("[Server] Kolízia! Hra končí.\n");
            state.is_running = 0;
        } else {
            update_snake_body(&state);

            if (state.snake_x == state.fruit_x && state.snake_y == state.fruit_y) {
                state.score++;
                state.snake_length++;
                place_fruit(&state);
            }
        }

        pipe_write(fd_write, &state, sizeof(GameState));
        usleep(MOVE_INTERVAL);
    }

    printf("[Server] Ukončujem server...\n");
    pipe_close(fd_read);
    pipe_close(fd_write);
    pipe_destroy(FIFO_CLIENT_TO_SERVER);
    pipe_destroy(FIFO_SERVER_TO_CLIENT);
    unlink("/tmp/server_ready");
}


