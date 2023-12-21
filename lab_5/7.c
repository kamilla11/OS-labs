#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
void checkFilesRecursive(const char* dirname,
    const char* outputFilename, long long sizeMin, long long sizeMax, time_t dateMin, time_t dateMax) {
    // Open the directory
    DIR* dir = opendir(dirname);
    if (dir == NULL) {
        printf("Error opening directory '%s'\n", dirname);
        exit(EXIT_FAILURE);
    }
    FILE* outputFile = fopen(outputFilename, "w");
    if (outputFile == NULL) {
        printf("Error opening output file '%s'\n", outputFilename);
        exit(EXIT_FAILURE);
    }
    // Read the directory entries
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        // Skip "." and ".." entries
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        // Construct the full path of the entry
        char fullPath[PATH_MAX];
        snprintf(fullPath, sizeof(fullPath), "%s/%s", dirname, entry->d_name);
        // Get file information
        struct stat fileStat;
        if (stat(fullPath, &fileStat) == -1) {
            printf("Error getting information for file '%s'\n", fullPath);
            continue;
        }
        // Check if it's a directory
        if (S_ISDIR(fileStat.st_mode)) {
            // Recursively list files in the subdirectory
            checkFilesRecursive(fullPath, outputFilename, sizeMin, sizeMax, dateMin, dateMax);
        }        else {
            // Check file size and creation date
            if ((fileStat.st_size >= sizeMin && fileStat.st_size <= sizeMax) &&
                (fileStat.st_ctime >= dateMin && fileStat.st_ctime <= dateMax)) {
               
                // Print to console
                printf("File: %-30s Size: %-10lld Creation Date: %s", entry->d_name, (long long)fileStat.st_size, ctime(&fileStat.st_ctime));
                // Print to file
                fprintf(outputFile, "File: %-30s Size: %-10lld Creation Date:     %s", entry->d_name, (long long)fileStat.st_size, ctime(&fileStat.st_ctime));
            }
        }
    }
    // Close the directory
    if (closedir(dir) == -1) {
        printf("Error closing directory '%s'\n", dirname);
        fclose(outputFile);
        exit(EXIT_FAILURE);
    }
    // Close the output file
    if (fclose(outputFile) == EOF) {
        printf("Error closing output file '%s'\n", outputFilename);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char* argv[]) {
    // Check the number of command line arguments
    if (argc != 7) {
        printf("Usage: %s <directory> <output_file> <size_min> <size_max> <date_min> <date_max>\n", argv[0]);
        return 1;
    }
    const char* dirname = argv[1];
    const char* outputFilename = argv[2];
    long long sizeMin = atoll(argv[3]);
    long long sizeMax = atoll(argv[4]);
    time_t dateMin = atoll(argv[5]);
    time_t dateMax = atoll(argv[6]);
    checkFilesRecursive(dirname, outputFilename, sizeMin, sizeMax, dateMin, dateMax);
    return 0;
}