#include <stdio.h>

enum OperationCode {
    OP_LOAD,    
    OP_STORE,   
    OP_ADD,    
    OP_SUB,  
    OP_HALT     
};

#define MEM_CAPACITY 10
#define REG_COUNT 2

typedef struct {
    enum OperationCode operation; 
    int firstOperand;   
    int secondOperand;   
} Command;

int memory[MEM_CAPACITY] = {0};  
int registers[REG_COUNT] = {0};  
int program_counter = 0; 


void perform_load(int firstOperand, int secondOperand);
void perform_store(int firstOperand, int secondOperand);
void perform_add(int firstOperand, int secondOperand);
void perform_sub(int firstOperand, int secondOperand);
void perform_halt();
void execute_program(Command program[], int program_size);
void test_program1();
void test_program2();

int main() {
    test_program1();
    printf("\n");
    test_program2();

    return 0;
}



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
    program_counter = -1; 
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



void test_program1() {
    printf("Executing Example Program 1\n");

    Command program[] = {
        {OP_LOAD, 0, 0},   
        {OP_LOAD, 1, 1},  
        {OP_ADD, 0, 1},   
        {OP_STORE, 2, 0}, 
        {OP_HALT, 0, 0}   
    };

    memory[0] = 5;
    memory[1] = 10;

    program_counter = 0; 
    execute_program(program, 5);
}


void test_program2() {
    printf("Executing Example Program 2\n");

    Command program[] = {
        {OP_LOAD, 0, 2},   
        {OP_LOAD, 1, 3},   
        {OP_SUB, 0, 1},    
        {OP_STORE, 4, 0},  
        {OP_HALT, 0, 0}    
    };

    memory[2] = 20;
    memory[3] = 7;

    program_counter = 0; 
    execute_program(program, 5);
}