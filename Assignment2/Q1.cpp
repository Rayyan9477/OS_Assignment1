#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>

const int MAX_PROCESSES = 100;
const int MAX_QUEUES = 4;

// Forward declarations
class Process;
class Queue;
void priorityScheduling(Queue& q);
void roundRobin(Queue& q);
void fcfs(Queue& q);
void readProcessesFromFile(const std::string& filename, std::vector<Process>& processes);
void printResults(const Queue& q);

class Process {
public:
    int pid;
    int arrival_time;
    int burst_time;
    int priority;
    int remaining_time;
    int waiting_time;
    int turnaround_time;

    Process(int p, int at, int bt, int pr)
        : pid(p), arrival_time(at), burst_time(bt), priority(pr),
          remaining_time(bt), waiting_time(0), turnaround_time(0) {}
};

class Queue {
public:
    std::queue<Process*> processes;
    std::string algorithm;
    int quantum;

    Queue(const std::string& algo, int q) : algorithm(algo), quantum(q) {}

    void enqueue(Process* p) {
        processes.push(p);
    }

    Process* dequeue() {
        if (processes.empty()) return nullptr;
        Process* p = processes.front();
        processes.pop();
        return p;
    }

    bool empty() const {
        return processes.empty();
    }

    size_t size() const {
        return processes.size();
    }
};

void priorityScheduling(Queue& q) {
    int current_time = 0;
    std::vector<Process*> temp_processes;
    while (!q.empty()) {
        Process* p = q.dequeue();
        temp_processes.push_back(p);
    }

    std::sort(temp_processes.begin(), temp_processes.end(),
              [](const Process* a, const Process* b) { return a->priority > b->priority; });

    for (Process* p : temp_processes) {
        p->waiting_time = current_time - p->arrival_time;
        current_time += p->burst_time;
        p->turnaround_time = current_time - p->arrival_time;
        p->remaining_time = 0;
        q.enqueue(p);
    }
}

void roundRobin(Queue& q) {
    int current_time = 0;
    std::queue<Process*> temp_queue = q.processes;
    q.processes = std::queue<Process*>();

    while (!temp_queue.empty()) {
        Process* p = temp_queue.front();
        temp_queue.pop();

        if (p->remaining_time > q.quantum) {
            current_time += q.quantum;
            p->remaining_time -= q.quantum;
            temp_queue.push(p);
        } else {
            current_time += p->remaining_time;
            p->waiting_time = current_time - p->arrival_time - p->burst_time;
            p->turnaround_time = current_time - p->arrival_time;
            p->remaining_time = 0;
            q.enqueue(p);
        }
    }
}

void fcfs(Queue& q) {
    int current_time = 0;
    std::queue<Process*> temp_queue = q.processes;
    q.processes = std::queue<Process*>();

    while (!temp_queue.empty()) {
        Process* p = temp_queue.front();
        temp_queue.pop();
        p->waiting_time = current_time - p->arrival_time;
        current_time += p->burst_time;
        p->turnaround_time = current_time - p->arrival_time;
        p->remaining_time = 0;
        q.enqueue(p);
    }
}

void readProcessesFromFile(const std::string& filename, std::vector<Process>& processes) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        exit(1);
    }

    int pid, arrival_time, burst_time, priority;
    while (file >> pid >> arrival_time >> burst_time >> priority) {
        processes.emplace_back(pid, arrival_time, burst_time, priority);
    }

    file.close();
}

void printResults(const Queue& q) {
    std::cout << "\nResults for " << q.algorithm << ":\n";
    std::cout << "PID\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time\n";

    float total_waiting_time = 0, total_turnaround_time = 0;
    int count = 0;

    std::queue<Process*> temp_queue = q.processes;
    while (!temp_queue.empty()) {
        Process* p = temp_queue.front();
        temp_queue.pop();
        std::cout << p->pid << "\t" << p->arrival_time << "\t\t" << p->burst_time << "\t\t"
                  << p->waiting_time << "\t\t" << p->turnaround_time << std::endl;
        total_waiting_time += p->waiting_time;
        total_turnaround_time += p->turnaround_time;
        count++;
    }

    std::cout << "Average Waiting Time: " << total_waiting_time / count << std::endl;
    std::cout << "Average Turnaround Time: " << total_turnaround_time / count << std::endl;
}

int main() {
    int num_queues;
    std::cout << "How many algorithms do you want to implement? ";
    std::cin >> num_queues;

    if (num_queues > MAX_QUEUES) {
        std::cerr << "Error: Maximum number of queues is " << MAX_QUEUES << std::endl;
        return 1;
    }

    std::vector<Queue> queues;
    std::vector<std::string> algorithms = {"Priority Scheduling", "Round Robin (Quantum=8)", "Round Robin (Quantum=10)", "FCFS"};
    std::vector<int> quantums = {0, 8, 10, 0};

    for (int i = 0; i < num_queues; i++) {
        queues.emplace_back(algorithms[i], quantums[i]);
    }

    std::vector<Process> processes;
    readProcessesFromFile("processes.txt", processes);

    // Distribute processes to queues
    for (auto& process : processes) {
        queues[0].enqueue(&process);
    }

    // Process queues
    for (int i = 0; i < num_queues; i++) {
        if (queues[i].algorithm == "Priority Scheduling") {
            priorityScheduling(queues[i]);
        } else if (queues[i].algorithm.find("Round Robin") != std::string::npos) {
            roundRobin(queues[i]);
        } else if (queues[i].algorithm == "FCFS") {
            fcfs(queues[i]);
        }

        printResults(queues[i]);

        // Move remaining processes to next queue
        if (i < num_queues - 1) {
            while (!queues[i].empty()) {
                Process* p = queues[i].dequeue();
                if (p->remaining_time > 0) {
                    queues[i+1].enqueue(p);
                }
            }
        }
    }

    return 0;
}