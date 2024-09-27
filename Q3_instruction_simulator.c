#include <stdio.h>
// Constants
enum OpCode {
    LOAD,    
    STORE,   
    ADD,    
    SUB,  
    HALT     
};

#define MEMORY_SIZE 10
#define REGISTER_COUNT 2

typedef struct {
    enum OpCode op; 
    int operand1;   
    int operand2;   
} Instruction;

int memory[MEMORY_SIZE] = {0};  
int registers[REGISTER_COUNT] = {0}; 
int program_counter = 0; 

void run_program(Instruction program[], int program_size);
void sample_program1();
void sample_program2();

int main() {
    sample_program1();
    sample_program2();

    return 0;
}

void run_program(Instruction program[], int program_size) {
    int running = 1;

    while (running && program_counter < program_size) {
        Instruction instr = program[program_counter];

        printf("Before execution of instruction at PC = %d\n", program_counter);
        printf("Registers: R0 = %d, R1 = %d\n", registers[0], registers[1]);
        printf("Memory: ");
        for (int i = 0; i < MEMORY_SIZE; i++) {
            printf("%d ", memory[i]);
        }
        printf("\n");

        switch (instr.op) {
            case LOAD:
                registers[instr.operand1] = memory[instr.operand2];
                printf("LOAD: R%d = memory[%d] = %d\n", instr.operand1, instr.operand2, registers[instr.operand1]);
                break;
            case STORE:
                memory[instr.operand1] = registers[instr.operand2];
                printf("STORE: memory[%d] = R%d = %d\n", instr.operand1, instr.operand2, memory[instr.operand1]);
                break;
            case ADD:
                registers[instr.operand1] += registers[instr.operand2];
                printf("ADD: R%d = R%d + R%d = %d\n", instr.operand1, instr.operand1, instr.operand2, registers[instr.operand1]);
                break;
            case SUB:
                registers[instr.operand1] -= registers[instr.operand2];
                printf("SUB: R%d = R%d - R%d = %d\n", instr.operand1, instr.operand1, instr.operand2, registers[instr.operand1]);
                break;
            case HALT:
                printf("HALT: Stopping execution\n");
                running = 0;
                break;
            default:
                printf("Unknown instruction!\n");
                running = 0;
                break;
        }

        program_counter++;

        printf("After execution of instruction at PC = %d\n", program_counter - 1);
        printf("Registers: R0 = %d, R1 = %d\n", registers[0], registers[1]);
        printf("Memory: ");
        for (int i = 0; i < MEMORY_SIZE; i++) {
            printf("%d ", memory[i]);
        }
        printf("\n\n");
    }
}

void sample_program1() {
    printf("Executing Sample Program 1\n");

    Instruction program[] = {
        {LOAD, 0, 0},   
        {LOAD, 1, 1},  
        {ADD, 0, 1},   
        {STORE, 2, 0}, 
        {HALT, 0, 0}  
    };

    memory[0] = 5;
    memory[1] = 10;

    run_program(program, 5);
}

void sample_program2() {
    printf("Executing Sample Program 2\n");

    Instruction program[] = {
        {LOAD, 0, 2},  
        {LOAD, 1, 3},  
        {SUB, 0, 1},    
        {STORE, 4, 0}, 
        {HALT, 0, 0}    
    };

    memory[2] = 20;
    memory[3] = 7;

    run_program(program, 5);
}