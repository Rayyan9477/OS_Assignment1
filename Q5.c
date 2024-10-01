#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to print left-aligned triangle
void print_left(int n) {
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= i; j++) {
            printf("* ");
        }
        printf("\n");
    }
}

// Function to print inverted full triangle
void print_inverted_full(int n) {
   for (int i = n; i >= 1; i--) {
        for (int j = n; j > i; j--) {
            printf("  "); // Space for alignment
        }
        for (int k = 1; k <= i; k++) {
            printf("* ");
        }
        printf("\n");
    }
}

// Function to print right-aligned triangle
void print_right(int n) {
    for (int i = 1; i <= n; i++) {
        for (int j = i; j < n; j++) {
            printf("  "); // Space for alignment
        }
        for (int k = 1; k <= i; k++) {
            printf("* ");
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Error: Invalid number of arguments.\n");
        printf("Usage: %s <pattern_option> <number>\n", argv[0]);
        return 1;
    }

    char *pattern_option = argv[1];
    int n = atoi(argv[2]);

    if (n <= 0) {
        printf("Error: The number of rows must be a positive integer.\n");
        return 1;
    }

    if (strcmp(pattern_option, "left") == 0) {
        print_left(n);
    } else if (strcmp(pattern_option, "inverted_full") == 0) {
        print_inverted_full(n);
    } else if (strcmp(pattern_option, "right") == 0) {
        print_right(n);
    } else {
        printf("Error: Invalid pattern option. Valid options are: left, inverted_full, right.\n");
        return 1;
    }

    return 0;
}
