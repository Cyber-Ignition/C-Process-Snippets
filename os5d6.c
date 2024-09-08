#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void reverseString(char *str) {
    int len = strlen(str);
    for (int i = 0; i<len/2; i++) {
        char temp = str[i];
        str[i] = str[len-i-1];
        str[len-i-1] = temp;
    }
}

void concatenateStrings(char *str1, const char *str2) {
    strcat(str1, str2);
}

int main() {
    char str1[100], str2[100];
    int len;
    pid_t pid1, pid2;

    printf("Enter a string: ");
    fgets(str1, sizeof(str1), stdin);
    str1[strcspn(str1, "\n")] = '\0';
    printf("P is running (PID: %d)\n", getpid());
    printf("Length of the string: %lu\n", strlen(str1));
    pid1 = fork();

    if (pid1 < 0) {
        perror("Fork failed");
        exit(1);
    }

    if (pid1 == 0) {
        reverseString(str1);
        printf("C1 is running (PID: %d)\nReversed string: %s\n", getpid(), str1);
        exit(0);
    } else {
        wait(NULL);

        printf("Enter another string to concatenate: ");
        fgets(str2, sizeof(str2), stdin);
        str2[strcspn(str2, "\n")] = '\0';

        pid2 = fork();

        if (pid2 < 0) {
            perror("Fork failed");
            exit(1);
        }

        if (pid2 == 0) {
            strcat(str1, str2);
            printf("C2 is running (PID: %d)\nConcatenated string: %s\n", getpid(), str1);
            exit(0);
        } else {
            wait(NULL);
        }
    }

    return 0;
}
