#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void calculateSumAndAverage(int marks[], int *sum, float *avg) {
    *sum = 0;
    for (int i = 0; i < 5; i++) {
        *sum += marks[i];
    }
    *avg = (float)(*sum) / 5;
}

int countSameMarks(int marks[]) {
    int count = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 5; j++) {
            if (marks[i] == marks[j]) {
                count++;
            }
        }
    }
    return count;
}

int sumEvenIndices(int marks[]) {
    int sum = 0;
    for (int i = 0; i < 5; i += 2) {
        sum += marks[i];
    }
    return sum;
}

int sumOddIndices(int marks[]) {
    int sum = 0;
    for (int i = 1; i < 5; i += 2) {
        sum += marks[i];
    }
    return sum;
}

int main() {
    int marks[5], sum;
    float avg;
    pid_t pid1, pid2, pid3;

    printf("Enter marks of 5 students:\n");
    for (int i = 0; i < 5; i++) {
        printf("Student %d: ", i + 1);
        scanf("%d", &marks[i]);
    }

    pid1 = fork();

    if (pid1 < 0) {
        perror("Fork failed");
        exit(1);
    }

    if (pid1 == 0) {
        pid2 = fork();

        if (pid2 < 0) {
            perror("Fork failed");
            exit(1);
        }

        if (pid2 == 0) {
            int evenSum = sumEvenIndices(marks);
            printf("C2 is running (PID: %d):\nSum of marks at even indices: %d\n", getpid(), evenSum);
            exit(0);
        } else {
            pid3 = fork();

            if (pid3 < 0) {
                perror("Fork failed");
                exit(1);
            }

            if (pid3 == 0) {
                int oddSum = sumOddIndices(marks);
                printf("C3 is running (PID: %d):\nSum of marks at odd indices: %d\n", getpid(), oddSum);
                exit(0);
            } else {
                wait(NULL);
                wait(NULL);
                int sameMarksCount = countSameMarks(marks);
                printf("C1 is running (PID: %d):\nNumber of students with the same marks: %d\n", getpid(), sameMarksCount);
                exit(0);
            }
        }
    } else {
        wait(NULL);
        pid2 = fork();

        if (pid2 < 0) {
            perror("Fork failed");
            exit(1);
        }

        if (pid2 == 0) {
            int evenSum = sumEvenIndices(marks);
            printf("C2 is running (PID: %d):\nSum of marks at even indices: %d\n", getpid(), evenSum);
            exit(0);
        } else {
            pid3 = fork();

            if (pid3 < 0) {
                perror("Fork failed");
                exit(1);
            }

            if (pid3 == 0) {
                int oddSum = sumOddIndices(marks);
                printf("C3 is running (PID: %d):\nSum of marks at odd indices: %d\n", getpid(), oddSum);
                exit(0);
            } else {
                wait(NULL);
                wait(NULL);
                calculateSumAndAverage(marks, &sum, &avg);
                printf("P is running (PID: %d):\nTotal sum of marks = %d\n", getpid(), sum);
                printf("Average marks = %.2f\n", avg);
            }
        }
    }

    return 0;
}
