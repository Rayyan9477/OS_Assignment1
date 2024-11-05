#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESSES 100
#define MAX_QUEUES 4

typedef struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int priority;
    int remaining_time;
    int waiting_time;
    int turnaround_time;
    struct Process* next;
} Process;

typedef struct Queue {
    Process* front;
    Process* rear;
    Process* completed;
    int count;
    char* algorithm;
    int quantum;
} Queue;

Queue queues[MAX_QUEUES];
Process processes[MAX_PROCESSES];
int num_processes = 0;

void initialize_queues(int num_queues, char* algorithms[], int quantums[]);
void enqueue_process(Queue* queue, Process* process);
Process* dequeue_process(Queue* queue);
void add_to_completed(Queue* queue, Process* process);
void preempt_process(Process** current_process, Process* new_process);
void read_processes_from_file(const char* filename);
void calculate_times(Process* process, int current_time);
void print_results(int num_queues);
void schedule_processes(int num_queues);

// Main Program
int main() {
    int num_queues;
    printf("How many algorithms do you want to implement? ");
    scanf("%d", &num_queues);

    if (num_queues > MAX_QUEUES) {
        printf("Error: Maximum number of queues is %d\n", MAX_QUEUES);
        return 1;
    }

    char* algorithms[] = {"Priority Scheduling", "Round Robin (Quantum=8)", "Round Robin (Quantum=10)", "FCFS"};
    int quantums[] = {0, 8, 10, 0};

    initialize_queues(num_queues, algorithms, quantums);
    read_processes_from_file("processes.txt");
    schedule_processes(num_queues);
    print_results(num_queues);

    return 0;
}

// Function definitions
void initialize_queues(int num_queues, char* algorithms[], int quantums[]) {
    for (int i = 0; i < num_queues; i++) {
        queues[i].front = NULL;
        queues[i].rear = NULL;
        queues[i].completed = NULL;
        queues[i].count = 0;
        queues[i].algorithm = algorithms[i];
        queues[i].quantum = quantums[i];
    }
}

void enqueue_process(Queue* queue, Process* process) {
    process->next = NULL;
    if (queue->rear == NULL) {
        queue->front = process;
        queue->rear = process;
    } else {
        queue->rear->next = process;
        queue->rear = process;
    }
    queue->count++;
}

Process* dequeue_process(Queue* queue) {
    if (queue->front == NULL) {
        return NULL;
    }
    Process* process = queue->front;
    queue->front = queue->front->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    queue->count--;
    return process;
}

void add_to_completed(Queue* queue, Process* process) {
    process->next = queue->completed;
    queue->completed = process;
}

void preempt_process(Process** current_process, Process* new_process) {
    enqueue_process(&queues[(*current_process)->priority], *current_process);
    *current_process = new_process;
}

void read_processes_from_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    while (fscanf(file, "%d %d %d %d", &processes[num_processes].pid, &processes[num_processes].arrival_time,
                  &processes[num_processes].burst_time, &processes[num_processes].priority) != EOF) {
        processes[num_processes].remaining_time = processes[num_processes].burst_time;
        processes[num_processes].waiting_time = 0;
        processes[num_processes].turnaround_time = 0;
        num_processes++;
    }

    fclose(file);
}

void calculate_times(Process* process, int current_time) {
    process->turnaround_time = current_time - process->arrival_time;
    process->waiting_time = process->turnaround_time - process->burst_time;
}

void print_results(int num_queues) {
    for (int i = 0; i < num_queues; i++) {
        printf("\nQueue %d (%s):\n", i + 1, queues[i].algorithm);
        Process* process = queues[i].completed;
        int total_waiting_time = 0;
        int total_turnaround_time = 0;
        int count = 0;

        while (process != NULL) {
            printf("PID: %d, Arrival Time: %d, Burst Time: %d, Waiting Time: %d, Turnaround Time: %d\n",
                   process->pid, process->arrival_time, process->burst_time, process->waiting_time, process->turnaround_time);
            total_waiting_time += process->waiting_time;
            total_turnaround_time += process->turnaround_time;
            count++;
            process = process->next;
        }

        if (count > 0) {
            printf("Average Waiting Time: %.2f\n", (float)total_waiting_time / count);
            printf("Average Turnaround Time: %.2f\n", (float)total_turnaround_time / count);
        }
        printf("\n");
    }
}

void schedule_processes(int num_queues) {
    Process* current_process = NULL;
    int current_time = 0;
    int quantum_counter = 0;

    while (1) {
        int all_done = 1;

        for (int i = 0; i < num_processes; i++) {
            if (processes[i].arrival_time == current_time) {
                enqueue_process(&queues[processes[i].priority], &processes[i]);
                printf("Process %d arrived and enqueued in queue %d\n", processes[i].pid, processes[i].priority);
            }
        }

        for (int i = 0; i < num_queues; i++) {
            if (queues[i].count > 0) {
                if (current_process == NULL || current_process->priority > i) {
                    if (current_process != NULL) {
                        preempt_process(&current_process, dequeue_process(&queues[i]));
                    } else {
                        current_process = dequeue_process(&queues[i]);
                    }
                    quantum_counter = 0;
                    break;
                }
            }
        }

        if (current_process != NULL) {
            current_process->remaining_time--;
            quantum_counter++;

            if (current_process->remaining_time == 0) {
                calculate_times(current_process, current_time + 1);
                add_to_completed(&queues[current_process->priority], current_process);
                printf("Process %d completed\n", current_process->pid);
                current_process = NULL;
            } else if (strcmp(queues[current_process->priority].algorithm, "Round Robin") == 0 &&
                       quantum_counter == queues[current_process->priority].quantum) {
                enqueue_process(&queues[current_process->priority], current_process);
                printf("Process %d quantum expired and re-enqueued in queue %d\n", current_process->pid, current_process->priority);
                current_process = NULL;
            }
        }

        for (int i = 0; i < num_processes; i++) {
            if (processes[i].remaining_time > 0) {
                all_done = 0;
                break;
            }
        }

        if (all_done) {
            break;
        }

        current_time++;
    }
}