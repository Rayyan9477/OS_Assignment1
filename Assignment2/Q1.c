#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESSES 100
#define MAX_QUEUES 4

// Forward declarations
typedef struct Process Process;
typedef struct Queue Queue;

struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int priority;
    int remaining_time;
    int waiting_time;
    int turnaround_time;
};

Queue* createQueue(const char* algorithm, int quantum);
void enqueue(Queue* q, Process* p);
Process* dequeue(Queue* q);
void priorityScheduling(Queue* q);
void roundRobin(Queue* q);
void fcfs(Queue* q);
void readProcessesFromFile(const char* filename, Process processes[], int* process_count);
void readProcessesManually(Process processes[], int* process_count);
void printResults(Queue* q);


struct Queue {
    Process* processes[MAX_PROCESSES];
    int front, rear;
    int size;
    char algorithm[50];
    int quantum;
};

Queue* createQueue(const char* algorithm, int quantum) {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = -1;
    q->size = 0;
    strcpy(q->algorithm, algorithm);
    q->quantum = quantum;
    return q;
}

void enqueue(Queue* q, Process* p) {
    if (q->rear == MAX_PROCESSES - 1) return;
    if (q->front == -1) q->front = 0;
    q->rear++;
    q->processes[q->rear] = p;
    q->size++;
}

Process* dequeue(Queue* q) {
    if (q->front == -1) return NULL;
    Process* p = q->processes[q->front];
    q->front++;
    if (q->front > q->rear) q->front = q->rear = -1;
    q->size--;
    return p;
}

int compare_priority(const void* a, const void* b) {
    return (*(Process**)b)->priority - (*(Process**)a)->priority;
}

void priorityScheduling(Queue* q) {
    int current_time = 0;
    qsort(q->processes, q->size, sizeof(Process*), compare_priority);

    for (int i = 0; i < q->size; i++) {
        Process* p = q->processes[i];
        p->waiting_time = current_time - p->arrival_time;
        current_time += p->burst_time;
        p->turnaround_time = current_time - p->arrival_time;
        p->remaining_time = 0;
    }
}

void roundRobin(Queue* q) {
    int current_time = 0;
    int completed = 0;
    while (completed < q->size) {
        for (int i = 0; i < q->size; i++) {
            Process* p = q->processes[i];
            if (p->remaining_time > 0) {
                if (p->remaining_time > q->quantum) {
                    current_time += q->quantum;
                    p->remaining_time -= q->quantum;
                } else {
                    current_time += p->remaining_time;
                    p->waiting_time = current_time - p->arrival_time - p->burst_time;
                    p->turnaround_time = current_time - p->arrival_time;
                    p->remaining_time = 0;
                    completed++;
                }
            }
        }
    }
}

void fcfs(Queue* q) {
    int current_time = 0;
    for (int i = 0; i < q->size; i++) {
        Process* p = q->processes[i];
        p->waiting_time = current_time - p->arrival_time;
        current_time += p->burst_time;
        p->turnaround_time = current_time - p->arrival_time;
        p->remaining_time = 0;
    }
}

void readProcessesFromFile(const char* filename, Process processes[], int* process_count) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file %s. Would you like to enter processes manually? (y/n): ", filename);
        char response;
        scanf(" %c", &response);
        if (response == 'y' || response == 'Y') {
            readProcessesManually(processes, process_count);
        } else {
            exit(1);
        }
        return;
    }

    *process_count = 0;
    while (fscanf(file, "%d %d %d %d", &processes[*process_count].pid, 
                  &processes[*process_count].arrival_time, 
                  &processes[*process_count].burst_time, 
                  &processes[*process_count].priority) == 4) {
        processes[*process_count].remaining_time = processes[*process_count].burst_time;
        processes[*process_count].waiting_time = 0;
        processes[*process_count].turnaround_time = 0;
        (*process_count)++;
    }

    fclose(file);
}

void readProcessesManually(Process processes[], int* process_count) {
    printf("Enter the number of processes: ");
    scanf("%d", process_count);

    for (int i = 0; i < *process_count; i++) {
        printf("Enter details for process %d (PID Arrival_Time Burst_Time Priority): ", i + 1);
        scanf("%d %d %d %d", &processes[i].pid, &processes[i].arrival_time, 
              &processes[i].burst_time, &processes[i].priority);
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].waiting_time = 0;
        processes[i].turnaround_time = 0;
    }
}

void printResults(Queue* q) {
    printf("\nResults for %s:\n", q->algorithm);
    printf("PID\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time\n");
    
    float total_waiting_time = 0, total_turnaround_time = 0;

    for (int i = 0; i < q->size; i++) {
        Process* p = q->processes[i];
        printf("%d\t%d\t\t%d\t\t%d\t\t%d\n", p->pid, p->arrival_time, p->burst_time, p->waiting_time, p->turnaround_time);
        total_waiting_time += p->waiting_time;
        total_turnaround_time += p->turnaround_time;
    }

    printf("Average Waiting Time: %.2f\n", total_waiting_time / q->size);
    printf("Average Turnaround Time: %.2f\n", total_turnaround_time / q->size);
}

int main() {
    int num_queues;
    printf("How many algorithms do you want to implement? ");
    scanf("%d", &num_queues);

    if (num_queues > MAX_QUEUES) {
        printf("Error: Maximum number of queues is %d\n", MAX_QUEUES);
        return 1;
    }

    Queue* queues[MAX_QUEUES];
    char* algorithms[] = {"Priority Scheduling", "Round Robin (Quantum=8)", "Round Robin (Quantum=10)", "FCFS"};
    int quantums[] = {0, 8, 10, 0};

    for (int i = 0; i < num_queues; i++) {
        queues[i] = createQueue(algorithms[i], quantums[i]);
    }

    Process processes[MAX_PROCESSES];
    int process_count;
    readProcessesFromFile("processes.txt", processes, &process_count);

    // Distribute processes to queues
    for (int i = 0; i < process_count; i++) {
        enqueue(queues[0], &processes[i]);
    }

    // Process queues
    for (int i = 0; i < num_queues; i++) {
        if (strcmp(queues[i]->algorithm, "Priority Scheduling") == 0) {
            priorityScheduling(queues[i]);
        } else if (strstr(queues[i]->algorithm, "Round Robin") != NULL) {
            roundRobin(queues[i]);
        } else if (strcmp(queues[i]->algorithm, "FCFS") == 0) {
            fcfs(queues[i]);
        }

        printResults(queues[i]);

        // Move remaining processes to next queue
        if (i < num_queues - 1) {
            for (int j = 0; j < queues[i]->size; j++) {
                Process* p = queues[i]->processes[j];
                if (p->remaining_time > 0) {
                    enqueue(queues[i+1], p);
                }
            }
        }
    }

    // Free allocated memory
    for (int i = 0; i < num_queues; i++) {
        free(queues[i]);
    }

    return 0;
}