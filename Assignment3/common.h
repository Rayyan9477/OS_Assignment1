// common.h
#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>

#define PIPE_A_TO_B "pipe_a_to_b"
#define PIPE_B_TO_A "pipe_b_to_a"
#define PID_FILE    "process_a.pid"
#define MAX_MSG_SIZE 256
#define NUM_MESSAGES 5

// Error handling macro
#define CHECK(x) \
    do { \
        if (!(x)) { \
            fprintf(stderr, "%s:%d: ", __FILE__, __LINE__); \
            perror(#x); \
            exit(EXIT_FAILURE); \
        } \
    } while (0)

#endif