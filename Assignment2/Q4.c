#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

#define NUM_EMPLOYEES 10
#define BUF_SIZE 256

struct PipePair {
    int owner_to_employee[2];
    int employee_to_owner[2];
};

void employee_process(struct PipePair *pipes, int id);
void owner_process(struct PipePair pipes[], int num_employees);


int main() {
    struct PipePair pipes[NUM_EMPLOYEES];
    pid_t pid;
    for (int i = 0; i < NUM_EMPLOYEES; i++) {
        if (pipe(pipes[i].owner_to_employee) < 0 || pipe(pipes[i].employee_to_owner) < 0) {
            perror("Pipe creation failed");
            exit(1);
        }
    }
    for (int i = 0; i < NUM_EMPLOYEES; i++) {
        pid = fork();
        if (pid < 0) {
            perror("Fork failed");
            exit(1);
        }
        if (pid == 0) {
            employee_process(&pipes[i], i + 1);
            exit(0);
        }
    }
    owner_process(pipes, NUM_EMPLOYEES);
    for (int i = 0; i < NUM_EMPLOYEES; i++) {
        wait(NULL);
    }
    return 0;
}

void employee_process(struct PipePair *pipes, int id) {
    char meeting_time[BUF_SIZE];
    char response[BUF_SIZE];
    close(pipes->owner_to_employee[1]);
    close(pipes->employee_to_owner[0]);
    read(pipes->owner_to_employee[0], meeting_time, BUF_SIZE);
    printf("Employee %d received meeting time: %s\n", id, meeting_time);
    srand(time(NULL) + id);
    int decision = rand() % 2;
    if (decision) {
        strcpy(response, "yes");
        printf("Employee %d: I can attend the meeting\n", id);
    } else {
        strcpy(response, "no");
        printf("Employee %d: Sorry, I cannot attend the meeting\n", id);
    }
    write(pipes->employee_to_owner[1], response, strlen(response) + 1);
    close(pipes->owner_to_employee[0]);
    close(pipes->employee_to_owner[1]);
}

void owner_process(struct PipePair pipes[], int num_employees) {
    char meeting_time[BUF_SIZE];
    char response[BUF_SIZE];
    int all_confirmed = 1;
    for (int i = 0; i < num_employees; i++) {
        close(pipes[i].owner_to_employee[0]);
        close(pipes[i].employee_to_owner[1]);
    }
    printf("Owner: Please suggest a meeting time: ");
    fgets(meeting_time, BUF_SIZE, stdin);
    meeting_time[strcspn(meeting_time, "\n")] = '\0';
    for (int i = 0; i < num_employees; i++) {
        write(pipes[i].owner_to_employee[1], meeting_time, strlen(meeting_time) + 1);
    }
    printf("\nWaiting for employee responses...\n\n");
    for (int i = 0; i < num_employees; i++) {
        read(pipes[i].employee_to_owner[0], response, BUF_SIZE);
        if (strcmp(response, "no") == 0) {
            all_confirmed = 0;
        }
    }
    printf("\nFinal Decision:\n");
    if (all_confirmed) {
        printf("Owner: Great! Meeting is confirmed for %s\n", meeting_time);
    } else {
        printf("Owner: Meeting cannot be scheduled. Not all employees can attend.\n");
    }
    for (int i = 0; i < num_employees; i++) {
        close(pipes[i].owner_to_employee[1]);
        close(pipes[i].employee_to_owner[0]);
    }
}