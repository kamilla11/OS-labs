#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
int listFilesInDirectory(const char* dirname) {
    // Open the directory
    DIR* dir = opendir(dirname);
    if (dir == NULL) {
        printf("Error opening directory\n");
        return 1;
    }
    printf("Contents of directory '%s':\n", dirname);
    // Read the directory entries
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }
    // Close the directory
    if (closedir(dir) == -1) {
        printf("Error closing directory\n");
        return 1;
    }
}
int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <directory >\n", argv[0]);
        return 1;
    }
    // Display contents of the current and specified directory directory
    if (listFilesInDirectory(".") != 0 || listFilesInDirectory(argv[1]) != 0) {
        return 1;
    }
    return 0;
}