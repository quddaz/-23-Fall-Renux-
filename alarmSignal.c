#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
char password[] = "456"; // 비밀번호 정의

void intHandler(int signo) {
    alarm(10);
    printf("10초 리셋\n");
}

void intHandler2(int signo) {
    printf("종료\n");
    exit(0);
}

int main() {
    char str[100];
    signal(SIGALRM, intHandler2);
    signal(SIGINT, intHandler);
    alarm(10);

    while (1) {
        scanf("%s", str); 
        if (strcmp(str, password) == 0) {
            printf("정답\n");
            exit(0);
        }
    }

    return 0;
}

