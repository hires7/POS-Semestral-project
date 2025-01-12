#include <stddef.h>
#ifndef IPC_H
#define IPC_H

#define FIFO_CLIENT_TO_SERVER "/tmp/hadik_fifo_client_to_server"
#define FIFO_SERVER_TO_CLIENT "/tmp/hadik_fifo_server_to_client"

#define MAP_WIDTH 10
#define MAP_HEIGHT 10

typedef enum {
    CMD_MOVE_UP,
    CMD_MOVE_DOWN,
    CMD_MOVE_LEFT,
    CMD_MOVE_RIGHT,
    CMD_PAUSE,
    CMD_EXIT
} Command;

typedef struct {
    int snake_x;
    int snake_y;
    int snake_length;
    int snake_body[MAP_WIDTH * MAP_HEIGHT][2];
    int fruit_x;
    int fruit_y;
    int score;
    int is_running;
    Command current_direction;
} GameState;


void pipe_init(const char *path);
void pipe_destroy(const char *path);
int pipe_open_write(const char *path);
int pipe_open_read(const char *path);
void pipe_close(int fd);
void pipe_write(int fd, const void *data, size_t size);
void pipe_read(int fd, void *buffer, size_t size);

#endif

