#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

#define NUM_EMPLOYEES 10
#define BUF_SIZE 256

// Structure to hold pipe pairs
struct PipePair {
    int owner_to_employee[2];  // Owner writes, Employee reads
    int employee_to_owner[2];  // Employee writes, Owner reads
};

void employee_process(struct PipePair *pipes, int id) {
    char meeting_time[BUF_SIZE];
    char response[BUF_SIZE];
    
    // Close unused pipe ends
    close(pipes->owner_to_employee[1]);
    close(pipes->employee_to_owner[0]);
    
    // Read meeting time from owner
    read(pipes->owner_to_employee[0], meeting_time, BUF_SIZE);
    printf("Employee %d received meeting time: %s\n", id, meeting_time);
    
    // Simulate employee decision (random response)
    srand(time(NULL) + id);
    int decision = rand() % 2;
    
    if (decision) {
        strcpy(response, "yes");
        printf("Employee %d: I can attend the meeting\n", id);
    } else {
        strcpy(response, "no");
        printf("Employee %d: Sorry, I cannot attend the meeting\n", id);
    }
    
    // Send response to owner
    write(pipes->employee_to_owner[1], response, strlen(response) + 1);
    
    // Close remaining pipe ends
    close(pipes->owner_to_employee[0]);
    close(pipes->employee_to_owner[1]);
}

void owner_process(struct PipePair pipes[], int num_employees) {
    char meeting_time[BUF_SIZE];
    char response[BUF_SIZE];
    int all_confirmed = 1;
    
    // Close unused pipe ends
    for (int i = 0; i < num_employees; i++) {
        close(pipes[i].owner_to_employee[0]);
        close(pipes[i].employee_to_owner[1]);
    }
    
    // Get meeting time from owner
    printf("Owner: Please suggest a meeting time: ");
    fgets(meeting_time, BUF_SIZE, stdin);
    meeting_time[strcspn(meeting_time, "\n")] = '\0';
    
    // Send meeting time to all employees
    for (int i = 0; i < num_employees; i++) {
        write(pipes[i].owner_to_employee[1], meeting_time, strlen(meeting_time) + 1);
    }
    
    // Wait for responses from all employees
    printf("\nWaiting for employee responses...\n\n");
    for (int i = 0; i < num_employees; i++) {
        read(pipes[i].employee_to_owner[0], response, BUF_SIZE);
        if (strcmp(response, "no") == 0) {
            all_confirmed = 0;
        }
    }
    
    // Announce final decision
    printf("\nFinal Decision:\n");
    if (all_confirmed) {
        printf("Owner: Great! Meeting is confirmed for %s\n", meeting_time);
    } else {
        printf("Owner: Meeting cannot be scheduled. Not all employees can attend.\n");
    }
    
    // Close remaining pipe ends
    for (int i = 0; i < num_employees; i++) {
        close(pipes[i].owner_to_employee[1]);
        close(pipes[i].employee_to_owner[0]);
    }
}

int main() {
    struct PipePair pipes[NUM_EMPLOYEES];
    pid_t pid;
    
    // Create pipes for each employee
    for (int i = 0; i < NUM_EMPLOYEES; i++) {
        if (pipe(pipes[i].owner_to_employee) < 0 || pipe(pipes[i].employee_to_owner) < 0) {
            perror("Pipe creation failed");
            exit(1);
        }
    }
    
    // Create employee processes
    for (int i = 0; i < NUM_EMPLOYEES; i++) {
        pid = fork();
        
        if (pid < 0) {
            perror("Fork failed");
            exit(1);
        }
        
        if (pid == 0) {  // Child process (employee)
            employee_process(&pipes[i], i + 1);
            exit(0);
        }
    }
    
    // Parent process (owner)
    owner_process(pipes, NUM_EMPLOYEES);
    
    // Wait for all children to finish
    for (int i = 0; i < NUM_EMPLOYEES; i++) {
        wait(NULL);
    }
    
    return 0;
}