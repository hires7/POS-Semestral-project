#include "ipc.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

void pipe_init(const char *path) {
    if (access(path, F_OK) == 0) {
        printf("[IPC] FIFO %s už existuje. Odstraňujem...\n", path);
        if (unlink(path) == -1) {
            perror("[IPC] Chyba pri odstraňovaní FIFO");
        }
    }

    if (mkfifo(path, 0666) == -1) {
        perror("[IPC] Chyba pri vytváraní FIFO");
        exit(EXIT_FAILURE);
    }
    printf("[IPC] FIFO %s bolo úspešne vytvorené.\n", path);
}

void pipe_destroy(const char *path) {
    if (access(path, F_OK) == 0) {
        if (unlink(path) == -1) {
            perror("[IPC] Chyba pri odstraňovaní FIFO");
        } else {
            printf("[IPC] FIFO %s bolo úspešne odstránené.\n", path);
        }
    } else {
        printf("[IPC] FIFO %s neexistuje, nie je potrebné ho odstraňovať.\n", path);
    }
}

int pipe_open_write(const char *path) {
    int fd = open(path, O_WRONLY);
    if (fd == -1) {
        perror("[IPC] Chyba pri otváraní FIFO na zápis");
        exit(EXIT_FAILURE);
    }
    return fd;
}

int pipe_open_read(const char *path) {
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        perror("[IPC] Chyba pri otváraní FIFO na čítanie");
        exit(EXIT_FAILURE);
    }
    return fd;
}

void pipe_close(int fd) {
    if (close(fd) == -1) {
        perror("[IPC] Chyba pri zatváraní FIFO");
        exit(EXIT_FAILURE);
    }
}

void pipe_write(int fd, const void *data, size_t size) {
    if (write(fd, data, size) == -1) {
        perror("[IPC] Chyba pri zápise do FIFO");
        exit(EXIT_FAILURE);
    }
}

void pipe_read(int fd, void *buffer, size_t size) {
    if (read(fd, buffer, size) == -1) {
        perror("[IPC] Chyba pri čítaní z FIFO");
        exit(EXIT_FAILURE);
    }
}

