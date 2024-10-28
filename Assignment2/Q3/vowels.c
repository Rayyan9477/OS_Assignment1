#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <ctype.h>

#define MAX_BUF 1024

void display_instructions() {
    printf("Vowel Counting Program\n");
    printf("This program counts the number of vowels in a given string.\n");
    printf("Instructions:\n");
    printf("1. Enter a string when prompted.\n");
    printf("2. The program will display the number of vowels in the entered string.\n");
}

int count_vowels(const char *str) {
    int count = 0;
    for (int i = 0; str[i]; i++) {
        char c = tolower(str[i]);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            count++;
        }
    }
    return count;
}

int main() {
    char buffer[MAX_BUF];

    display_instructions();

    printf("Please enter a string: ");
    if (fgets(buffer, MAX_BUF, stdin) != NULL) {
        // Remove newline character if present
        buffer[strcspn(buffer, "\n")] = '\0';

        int vowel_count = count_vowels(buffer);
        printf("The number of vowels in the entered string is: %d\n", vowel_count);
    } else {
        printf("Error reading input.\n");
    }

    return 0;
}