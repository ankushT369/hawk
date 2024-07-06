#include <stdio.h>
#include <stdlib.h>

long get_char_offset(const char *filename, char target) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return -1;
    }

    long offset = -1;
    char c;
    while ((c = fgetc(file)) != EOF) {
        if (c == target) {
            offset = ftell(file) - 1; // Subtract 1 because ftell gives the position after fgetc
            break;
        }
    }

    fclose(file);
    return offset;
}

int main() {
    const char *filename = "test.txt";
    char target = 'q';
    
    long offset = get_char_offset(filename, target);
    if (offset != -1) {
        printf("Character '%c' found at offset: %#lx\n", target, offset);
    } else {
        printf("Character '%c' not found in the file.\n", target);
    }

    return 0;
}

