#include <stdio.h>
#include <stdlib.h>
int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    char* filename = argv[1];
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file");
        return 1;
    }
    char ch;
    printf("Enter text (Ctrl+F to exit):\n");
    while ((ch = getc(stdin)) != EOF) {
        if (putc(ch, file) == EOF) {
            printf("Error writing to file");
            fclose(file);
            return 1;
        }
        if (ch == 6) { //ASCII code for Ctrl+F
            break;
        }
    }
    if (fclose(file) == EOF) {
        printf("Error closing file");
        return 1;
    }
    printf("File '%s' written successfully.\n", filename);
    return 0;
}