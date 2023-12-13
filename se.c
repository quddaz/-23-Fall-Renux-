#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#define MAX_LINE 256
#define CLIENT_NAME_MAX_LEN 20

// 각 클라이언트의 ID
#define C1_ID 1
#define C2_ID 2

void generateNickname(char* nick) {
    snprintf(nick, CLIENT_NAME_MAX_LEN, "User%d", rand() % 1000);
}

void* fifoFunc(void* arg) {
    int id = *((int*)arg);

    char readFifo[CLIENT_NAME_MAX_LEN];
    char writeFifo[CLIENT_NAME_MAX_LEN];
    snprintf(readFifo, sizeof(readFifo), "./cfifo%d", id * 2 - 1);
    snprintf(writeFifo, sizeof(writeFifo), "./cfifo%d", id * 2);

    int fd[2];
    int n;

    if (mkfifo(readFifo, 0660) == -1 && errno != EEXIST) {
        perror("mkfifo");
        exit(id + 1);
    }
    if (mkfifo(writeFifo, 0660) == -1 && errno != EEXIST) {
        perror("mkfifo");
        exit(id + 3);
    }

    fd[0] = open(readFifo, O_RDONLY);
    fd[1] = open(writeFifo, O_WRONLY);

    if (fd[0] == -1 || fd[1] == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    printf("%d connected\n", id);

    char msg[MAX_LINE];

    char nick[CLIENT_NAME_MAX_LEN];
    generateNickname(nick);

    while (1) {
        n = read(fd[0], msg, MAX_LINE);

        if (n == -1 && errno == EAGAIN) {
            continue;
        } else if (n <= 0) {
            break;
        }

        if (strncmp(msg, "exit", 4) == 0) {
            exit(0);
        }

        char formattedMsg[MAX_LINE];
        snprintf(formattedMsg, sizeof(formattedMsg), "[%s]: %s", nick, msg);

        write(fd[1], formattedMsg, strlen(formattedMsg) + 1);
    }

    close(fd[0]);
    close(fd[1]);
    return NULL;
}

int main() {
    pthread_t threads[2];

    printf("Server\n");

    int client1ID = C1_ID;
    int client2ID = C2_ID;

    pthread_create(&threads[0], NULL, fifoFunc, (void*)&client1ID);
    pthread_create(&threads[1], NULL, fifoFunc, (void*)&client2ID);

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    return 0;
}