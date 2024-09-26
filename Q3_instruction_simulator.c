#include <stdio.h>

// Enum to represent different opcodes
enum OpCode {
    LOAD,    // Load value from memory to register
    STORE,   // Store value from register to memory
    ADD,     // Add two registers
    SUB,     // Subtract two registers
    HALT     // Stop program execution
};

// Memory size and register count
#define MEMORY_SIZE 10
#define REGISTER_COUNT 2
// Structure to represent an instruction
typedef struct {
    enum OpCode op; // Operation code
    int operand1;   // First operand (register or memory address)
    int operand2;   // Second operand (optional)
} Instruction;

// Global variables for memory, registers, and program counter
int memory[MEMORY_SIZE] = {0};  // Memory
int registers[REGISTER_COUNT] = {0};  // Registers
int program_counter = 0; // Program Counter

// Function to simulate the fetch-decode-execute cycle
void run_program(Instruction program[], int program_size) {
    int running = 1;

    while (running && program_counter < program_size) {
        // Fetch: Get the next instruction
        Instruction instr = program[program_counter];

        // Print the state before execution
        printf("Before execution of instruction at PC = %d\n", program_counter);
        printf("Registers: R0 = %d, R1 = %d\n", registers[0], registers[1]);
        printf("Memory: ");
        for (int i = 0; i < MEMORY_SIZE; i++) {
            printf("%d ", memory[i]);
        }
        printf("\n");

        // Decode and Execute: Perform the operation based on the opcode
        switch (instr.op) {
            case LOAD:
                // LOAD register from memory (LOAD R0, addr)
                registers[instr.operand1] = memory[instr.operand2];
                printf("LOAD: R%d = memory[%d] = %d\n", instr.operand1, instr.operand2, registers[instr.operand1]);
                break;
            case STORE:
                // STORE register to memory (STORE addr, R0)
                memory[instr.operand1] = registers[instr.operand2];
                printf("STORE: memory[%d] = R%d = %d\n", instr.operand1, instr.operand2, memory[instr.operand1]);
                break;
            case ADD:
                // ADD register1 and register2, store result in register1 (ADD R0, R1)
                registers[instr.operand1] += registers[instr.operand2];
                printf("ADD: R%d = R%d + R%d = %d\n", instr.operand1, instr.operand1, instr.operand2, registers[instr.operand1]);
                break;
            case SUB:
                // SUB register2 from register1, store result in register1 (SUB R0, R1)
                registers[instr.operand1] -= registers[instr.operand2];
                printf("SUB: R%d = R%d - R%d = %d\n", instr.operand1, instr.operand1, instr.operand2, registers[instr.operand1]);
                break;
            case HALT:
                // HALT the program
                printf("HALT: Stopping execution\n");
                running = 0;
                break;
            default:
                printf("Unknown instruction!\n");
                running = 0;
                break;
        }

        // Increment the program counter
        program_counter++;

        // Print the state after execution
        printf("After execution of instruction at PC = %d\n", program_counter - 1);
        printf("Registers: R0 = %d, R1 = %d\n", registers[0], registers[1]);
        printf("Memory: ");
        for (int i = 0; i < MEMORY_SIZE; i++) {
            printf("%d ", memory[i]);
        }
        printf("\n\n");
    }
}

// Sample program 1: Load, Add, Store, and Halt
void sample_program1() {
    printf("Executing Sample Program 1\n");

    Instruction program[] = {
        {LOAD, 0, 0},   // LOAD R0, memory[0]
        {LOAD, 1, 1},   // LOAD R1, memory[1]
        {ADD, 0, 1},    // ADD R0, R1 (R0 = R0 + R1)
        {STORE, 2, 0},  // STORE memory[2], R0
        {HALT, 0, 0}    // HALT
    };

    memory[0] = 5;
    memory[1] = 10;

    run_program(program, 5);
}

// Sample program 2: Load, Subtract, Store, and Halt
void sample_program2() {
    printf("Executing Sample Program 2\n");

    Instruction program[] = {
        {LOAD, 0, 2},   // LOAD R0, memory[2]
        {LOAD, 1, 3},   // LOAD R1, memory[3]
        {SUB, 0, 1},    // SUB R0, R1 (R0 = R0 - R1)
        {STORE, 4, 0},  // STORE memory[4], R0
        {HALT, 0, 0}    // HALT
    };

    memory[2] = 20;
    memory[3] = 7;

    run_program(program, 5);
}

int main() {
    // Run sample programs
    sample_program1();
    sample_program2();

    return 0;
}
