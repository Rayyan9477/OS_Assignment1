# Operating Systems and Shell Scripting

This repository contains assignments related to Operating Systems. The main focus is on understanding system-level programming, including shell scripting and signal handling in C.

## Importance

The assignments in this repository demonstrate crucial concepts in Operating Systems, which are essential for various fields, including machine learning and data science. Understanding these concepts helps in optimizing performance, managing resources, and ensuring efficient execution of applications.

## Repository Contents

- **OS_Assignment1_Q2.sh**: A shell script that includes various functions such as creating a new user, listing installed applications, installing Dropbox, and configuring network settings.
- **Q1lab.c**: A C program that handles different signals (`SIGINT`, `SIGTERM`, `SIGALRM`) and demonstrates signal handling by registering custom signal handlers.

## Usage in Machine Learning and Data Science

- **Resource Management**: The knowledge of operating systems helps in managing computational resources efficiently, which is crucial for training large machine learning models.
- **Automation**: Shell scripting can automate repetitive tasks, such as data preprocessing and environment setup, which is beneficial in data science workflows.
- **Performance Optimization**: Understanding and handling system signals can help in creating robust machine learning applications that can handle interruptions and optimize performance.

## Shell Scripting

The `OS_Assignment1_Q2.sh` script leverages shell scripting to perform various system-level tasks:
- **User Management**: Automates user creation and admin privilege assignment.
- **Application Management**: Lists installed applications and installs new ones, like Dropbox.
- **Network Configuration**: Configures network settings for the system.
- **Help Display**: Provides a help guide for using the script with different options.

## Signal Handling in C

The `Q1lab.c` program demonstrates signal handling in C:
- **Custom Signal Handlers**: Registers handlers for `SIGINT`, `SIGTERM`, and `SIGALRM` signals.
- **Graceful Termination**: Handles termination signals to ensure the program exits gracefully.
- **Alarm Handling**: Demonstrates handling of alarm signals for timed events.

## Getting Started

Clone this repository to your local machine and navigate to the project directory:

```sh
git clone https://github.com/Rayyan9477/xxxxxx.git
cd OS_Assignment1
```

### Running the Shell Script

To execute the shell script with different options, use the following commands:

- Create a new user and assign admin privileges:
  ```sh
  ./OS_Assignment1_Q2.sh -uc
  ```

- List all installed applications:
  ```sh
  ./OS_Assignment1_Q2.sh -ld
  ```

- Install Dropbox:
  ```sh
  ./OS_Assignment1_Q2.sh -ins
  ```

- Configure network settings:
  ```sh
  ./OS_Assignment1_Q2.sh -ipcon
  ```

- Display help information:
  ```sh
  ./OS_Assignment1_Q2.sh -help
  ```

### Running the C Program

Compile and run the C program to demonstrate signal handling:

```sh
gcc -o Q1lab Q1lab.c
./Q1lab
```

Send signals to the running program using the `kill` command:

```sh
kill -SIGINT <pid>
kill -SIGTERM <pid>
kill -SIGALRM <pid>
```

Replace `<pid>` with the process ID of the running program.

## Conclusion

This repository provides hands-on experience with essential operating system concepts, which are valuable for optimizing and managing resources in machine learning and data science applications. Shell scripting and signal handling are powerful tools for automating tasks and ensuring robust application behavior.

Feel free to explore and contribute to this repository!

## Contact
#### Rayyan Ahmed 
#### [LinkedIn](https://www.linkedin.com/in/rayyan-ahmed9477/)
#### Check out more of my work at:  [GitHub](https://github.com/Rayyan9477)
#### Email: rayyanahmed265@yahoo.com
