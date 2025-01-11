#include <pthread.h>
#include "ipc.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

typedef struct {
    int fd_write;
    int fd_read;
} ClientArgs;

void *handle_input(void *arg) {
    ClientArgs *args = (ClientArgs *)arg;
    int fd_write = args->fd_write;
    Command command;
    while (1) {
        char input = getchar();
        switch (input) {
            case 'w': command = CMD_MOVE_UP; break;
            case 's': command = CMD_MOVE_DOWN; break;
            case 'a': command = CMD_MOVE_LEFT; break;
            case 'd': command = CMD_MOVE_RIGHT; break;
            case 'p': command = CMD_PAUSE; break;
            case 'q': command = CMD_EXIT; break;
            default: continue;
        }
        pipe_write(fd_write, &command, sizeof(Command));
        if (command == CMD_EXIT) break;
    }
    return NULL;
}

void *receive_and_render(void *arg) {
    ClientArgs *args = (ClientArgs *)arg;
    int fd_read = args->fd_read;
    GameState state;
    while (1) {
        pipe_read(fd_read, &state, sizeof(GameState));
        system("clear");
        printf("Score: %d\n", state.score);
        for (int y = 0; y < MAP_HEIGHT; y++) {
            for (int x = 0; x < MAP_WIDTH; x++) {
                if (x == state.snake_x && y == state.snake_y) printf("O ");
                else if (x == state.fruit_x && y == state.fruit_y) printf("* ");
                else {
                    int is_body = 0;
                    for (int i = 0; i < state.snake_length; i++) {
                        if (x == state.snake_body[i][0] && y == state.snake_body[i][1]) {
                            printf("o ");
                            is_body = 1;
                            break;
                        }
                    }
                    if (!is_body) printf(". ");
                }
            }
            printf("\n");
        }
        if (!state.is_running) break;
    }
    return NULL;
}

void init_client() {
    int fd_write = pipe_open_write(FIFO_CLIENT_TO_SERVER);
    int fd_read = pipe_open_read(FIFO_SERVER_TO_CLIENT);

    ClientArgs args = {fd_write, fd_read};

    pthread_t thread_input, thread_render;

    pthread_create(&thread_input, NULL, handle_input, &args);
    pthread_create(&thread_render, NULL, receive_and_render, &args);

    pthread_join(thread_input, NULL);
    pthread_join(thread_render, NULL);

    pipe_close(fd_write);
    pipe_close(fd_read);
}


