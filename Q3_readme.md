# C Language Intsruction Simulator

This program is a basic simulator that shows how a simple computer processor works by performing basic operations like loading, storing, adding, and subtracting values from memory and registers. It simulates the fetch-decode-execute cycle, where the processor fetches an instruction, understands (decodes) it, and then carries it out (executes it).

## Features
- **Memory & Registers:**  
  The program has:
  - **Memory:** 10 slots where numbers can be stored.
  - **Registers:** 2 registers (R0 and R1), which are small storage spaces inside the processor.

- **Operations (OpCodes):**  
  The program can perform these operations:
  - **LOAD:** Loads a value from memory into a register.
  - **STORE:** Stores a value from a register into memory.
  - **ADD:** Adds two values from registers and saves the result in one of them.
  - **SUB:** Subtracts one value in a register from another and saves the result.
  - **HALT:** Stops the program.

## Memory Layout
The memory is an array (list) with 10 spots, all set to zero at the start. Registers R0 and R1 are also set to zero. A special variable called the **program counter** keeps track of which instruction is being executed.

### Memory Example:
- `memory[0]`: Holds a number (e.g., 5).
- `registers[0]`: Holds a number (e.g., 10).

## How to Compile and Run

1. **Compile the Program:**
   ```
   gcc -o simulator simulator.c
   ```

2. **Run the Program:**
   ```
   ./simulator
   ```

   This will run two example programs that show how the simulator works.

## Example Outputs

### Output of Example Program 1:
```
Executing Example Program 1
LOAD: R0 = memory[0] -> 5
LOAD: R1 = memory[1] -> 10
ADD: R0 = R0 + R1 -> 15
STORE: memory[2] = R0 -> 15
HALT
```

### Output of Example Program 2:
```
Executing Example Program 2
LOAD: R0 = memory[2] -> 20
LOAD: R1 = memory[3] -> 7
SUB: R0 = R0 - R1 -> 13
STORE: memory[4] = R0 -> 13
HALT
```

## Operation Codes (OpCodes)
Each instruction (or command) has an operation code and two numbers:
- The **first number** usually refers to a register.
- The **second number** usually refers to a memory slot or another register.

### Supported Operations:
1. **LOAD:** Load a value from memory into a register.
   - Example: `{OP_LOAD, 0, 0}` loads the value in `memory[0]` into `register[0]`.
2. **STORE:** Store a value from a register into memory.
   - Example: `{OP_STORE, 2, 0}` stores the value in `register[0]` into `memory[2]`.
3. **ADD:** Add values in two registers and save the result in the first register.
   - Example: `{OP_ADD, 0, 1}` adds the values in `register[0]` and `register[1]`, and stores the result in `register[0]`.
4. **SUB:** Subtract the value of one register from another and store the result.
   - Example: `{OP_SUB, 0, 1}` subtracts the value in `register[1]` from `register[0]`, and stores the result in `register[0]`.
5. **HALT:** Stops the program from running.
   - Example: `{OP_HALT, 0, 0}` stops the program.

## Example Programs

### Example Program 1:
1. Load the value from `memory[0]` into `R0`.
2. Load the value from `memory[1]` into `R1`.
3. Add `R0` and `R1`, store the result in `R0`.
4. Store the value from `R0` into `memory[2]`.
5. Stop the program.

### Example Program 2:
1. Load the value from `memory[2]` into `R0`.
2. Load the value from `memory[3]` into `R1`.
3. Subtract `R1` from `R0`, store the result in `R0`.
4. Store the value from `R0` into `memory[4]`.
5. Stop the program.

## How the Program Works (Fetch-Decode-Execute Cycle)
1. **Fetch:** The program counter fetches (gets) the next command.
2. **Decode:** The program reads the command and decides what to do.
3. **Execute:** The program performs the operation, like loading or adding values.

## Conclusion
This program shows how a simple processor can execute basic operations. It demonstrates key concepts like how to load and store data, perform addition and subtraction, and halt the program.