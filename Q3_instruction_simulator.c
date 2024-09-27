#include <stdio.h>

// Enum to represent different opcodes
enum OperationCode {
    OP_LOAD,    
    OP_STORE,   
    OP_ADD,    
    OP_SUB,  
    OP_HALT     
};

// Memory size and register count
#define MEM_CAPACITY 10
#define REG_COUNT 2

// Structure to represent an instruction
typedef struct {
    enum OperationCode operation; 
    int firstOperand;   
    int secondOperand;   
} Command;

// Global variables for memory, registers, and program counter
int memory[MEM_CAPACITY] = {0};  // Memory array
int registers[REG_COUNT] = {0};  // Register array
int program_counter = 0; // Program Counter

// Forward declarations
void perform_load(int firstOperand, int secondOperand);
void perform_store(int firstOperand, int secondOperand);
void perform_add(int firstOperand, int secondOperand);
void perform_sub(int firstOperand, int secondOperand);
void perform_halt();
void execute_program(Command program[], int program_size);
void example_program1();
void example_program2();

int main() {
    example_program1();
    example_program2();

    return 0;
}

// Function implementations
void perform_load(int firstOperand, int secondOperand) {
    registers[firstOperand] = memory[secondOperand];
    printf("LOAD: R%d = memory[%d] -> %d\n", firstOperand, secondOperand, registers[firstOperand]);
}

void perform_store(int firstOperand, int secondOperand) {
    memory[firstOperand] = registers[secondOperand];
    printf("STORE: memory[%d] = R%d -> %d\n", firstOperand, secondOperand, memory[firstOperand]);
}

void perform_add(int firstOperand, int secondOperand) {
    registers[firstOperand] += registers[secondOperand];
    printf("ADD: R%d = R%d + R%d -> %d\n", firstOperand, firstOperand, secondOperand, registers[firstOperand]);
}

void perform_sub(int firstOperand, int secondOperand) {
    registers[firstOperand] -= registers[secondOperand];
    printf("SUB: R%d = R%d - R%d -> %d\n", firstOperand, firstOperand, secondOperand, registers[firstOperand]);
}

void perform_halt() {
    program_counter = -1; // Set program counter to an invalid value to stop execution
    printf("HALT\n");
}

void execute_program(Command program[], int program_size) {
    int running = 1;

    while (running && program_counter < program_size) {
        Command current_command = program[program_counter];
        switch (current_command.operation) {
            case OP_LOAD:
                perform_load(current_command.firstOperand, current_command.secondOperand);
                break;
            case OP_STORE:
                perform_store(current_command.firstOperand, current_command.secondOperand);
                break;
            case OP_ADD:
                perform_add(current_command.firstOperand, current_command.secondOperand);
                break;
            case OP_SUB:
                perform_sub(current_command.firstOperand, current_command.secondOperand);
                break;
            case OP_HALT:
                perform_halt();
                running = 0;
                break;
        }
        program_counter++;
    }
}

// Example program 1: Load, Add, Store, and Halt
void example_program1() {
    printf("Executing Example Program 1\n");

    Command program[] = {
        {OP_LOAD, 0, 0},   // LOAD R0, memory[0]
        {OP_LOAD, 1, 1},   // LOAD R1, memory[1]
        {OP_ADD, 0, 1},    // ADD R0, R1 (R0 = R0 + R1)
        {OP_STORE, 2, 0},  // STORE memory[2], R0
        {OP_HALT, 0, 0}    // HALT
    };

    memory[0] = 5;
    memory[1] = 10;

    program_counter = 0; // Reset program counter
    execute_program(program, 5);
}

// Example program 2: Load, Subtract, Store, and Halt
void example_program2() {
    printf("Executing Example Program 2\n");

    Command program[] = {
        {OP_LOAD, 0, 2},   // LOAD R0, memory[2]
        {OP_LOAD, 1, 3},   // LOAD R1, memory[3]
        {OP_SUB, 0, 1},    // SUB R0, R1 (R0 = R0 - R1)
        {OP_STORE, 4, 0},  // STORE memory[4], R0
        {OP_HALT, 0, 0}    // HALT
    };

    memory[2] = 20;
    memory[3] = 7;

    program_counter = 0; // Reset program counter
    execute_program(program, 5);
}