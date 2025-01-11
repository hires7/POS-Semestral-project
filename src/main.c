#include "client.h"
#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/wait.h>

int main() {
    int vyber;
    printf("Vyberte režim:\n1. Klient a Server\n2. Iba Server\n3. Iba Klient\n> ");
    scanf("%d", &vyber);

    if (vyber == 1) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("Chyba pri forku");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            printf("[Klient] Spúšťam klienta...\n");
            init_client();
            exit(EXIT_SUCCESS);
        } else {
            printf("[Server] Spúšťam server...\n");
            init_server();
            wait(NULL);
        }
    } else if (vyber == 2) {
        printf("[Server] Spúšťam iba server...\n");
        init_server();
    } else if (vyber == 3) {
        printf("[Klient] Spúšťam iba klienta...\n");
        init_client();
    } else {
        printf("Neplatná voľba.\n");
    }

    return 0;
}
