#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <source_file> <destination_file>\n", argv[0]);
        return 1;
    }
    const char* sourceFilename = argv[1];
    const char* destinationFilename = argv[2];
    // Open the source file for reading
    FILE* sourceFile = fopen(sourceFilename, "r");
    if (sourceFile == NULL) {
        printf("Error opening source file for reading\n");
        return 1;
    }
    // Open the destination file for writing
    FILE* destinationFile = fopen(destinationFilename, "w");
    if (destinationFile == NULL) {
        printf("Error opening destination file for writing\n");
        fclose(sourceFile);
        return 1;
    }
    // Copy the contents of the source file to the destination file
    int ch;
    while ((ch = fgetc(sourceFile)) != EOF) {
        if (fputc(ch, destinationFile) == EOF) {
            printf("Error writing to destination file\n");
            fclose(sourceFile);
            fclose(destinationFile);
            return 1;
        }
    }
    // Close the files
    if (fclose(sourceFile) == EOF) {
        printf("Error closing source file\n");
        fclose(destinationFile);
        return 1;
    }
    if (fclose(destinationFile) == EOF) {
        printf("Error closing destination file\n");
        return 1;
    }
    // Copy file access rights
    struct stat sourceStat;
    if (stat(sourceFilename, &sourceStat) == -1) {
        printf("Error getting source file information\n");
        return 1;
    }
    if (chmod(destinationFilename, sourceStat.st_mode) == -1) {
        printf("Error copying file permissions\n");
        return 1;
    }
    printf("File '%s' successfully copied to '%s'.\n", sourceFilename, destinationFilename);
    return 0;
}