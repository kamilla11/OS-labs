#include <stdio.h>
#include <stdlib.h>
void displayLines(FILE* file, int numLines) {
    int ch;
    int linesDisplayed = 0;
    while ((ch = fgetc(file)) != EOF) {
        // Display the character
        putchar(ch);

        // Count lines
        if (ch == '\n') {
            linesDisplayed++;

            // Check if the desired number of lines is displayed
            if (numLines > 0 && linesDisplayed >= numLines) {
                // Wait for user input to display the next group
                getchar();
                linesDisplayed = 0;
            }
        }
    }
}
int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <filename> <N>\n", argv[0]);
        return 1;
    }

    char* filename = argv[1];
    int numLines = atoi(argv[2]);

    // Open the file for reading
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file for reading\n");
        return 1;
    }

    printf("File contents:\n");
    displayLines(file, numLines);
    // Close the file
    if (fclose(file) == EOF) {
        printf("Error closing file\n");
        return 1;
    }
    return 0;
}