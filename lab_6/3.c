#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <dirent.h>

#include <unistd.h>

#include <sys/types.h>

#include <sys/wait.h>

#define MAX_BYTES 255

int searchInFile(const char *filename, const char *targetBytes, int maxBytes)

{

  FILE *file = fopen(filename, "rb");

  if (file == NULL)

  {

    printf("Error opening file: %s\n", filename);

    return 1;
  }

  char buffer[MAX_BYTES];

  int bytesRead = 0;

  int totalBytesRead = 0;

  int matches = 0;

  while ((bytesRead = fread(buffer, 1, maxBytes, file)) > 0)

  {

    totalBytesRead += bytesRead;

    for (int i = 0; i < bytesRead; i++)

    {

      if (memcmp(buffer + i, targetBytes, maxBytes) == 0)

      {

        matches++;
      }
    }
  }

  printf("PID: %d, File: %s, Total Bytes Viewed: %d, Matches: %d\n",

         getpid(), filename, totalBytesRead, matches);

  if (fclose(file) != 0)

  {

    printf("Error closing file: %s\n", filename);

    return 1;
  }

  return 0;
}

int searchInDirectory(const char *dirname, const char *targetBytes, int maxBytes, int maxProcesses)

{

  DIR *dir = opendir(dirname);

  if (dir == NULL)

  {

    printf("Error opening directory: %s\n", dirname);

    return 1;
  }

  struct dirent *entry;

  int currentProcesses = 0;

  while ((entry = readdir(dir)) != NULL)

  {

    if (entry->d_type == DT_REG)

    {

      char filePath[PATH_MAX];

      snprintf(filePath, PATH_MAX, "%s/%s", dirname, entry->d_name);

      pid_t pid = fork();

      if (pid < 0)

      {

        printf("Error forking process\n");

        return 1;
      }

      else if (pid == 0)

      {

        // Child process

        return searchInFile(filePath, targetBytes, maxBytes);
      }

      else

      {

        // Parent process

        currentProcesses++;

        if (currentProcesses >= maxProcesses)

        {

          wait(NULL); // Waiting for the completion of one of the child processes

          currentProcesses--;
        }
      }
    }
  }

  if (closedir(dir) != 0)

  {

    printf("Error closing directory: %s\n", dirname);

    return 1;
  }

  // Waiting for the completion of the remaining child processes

  while (wait(NULL) > 0)

  {

    currentProcesses--;
  }

  return 0;
}

int main()

{

  char dirname[PATH_MAX];

  char targetBytes[MAX_BYTES];

  int maxProcesses;

  printf("Enter the directory name: ");

  scanf("%s", dirname);

  printf("Enter the target bytes (up to 255 bytes): ");

  scanf("%s", targetBytes);

  printf("Enter the maximum number of child processes: ");

  scanf("%d", &maxProcesses);

  return searchInDirectory(dirname, targetBytes, strlen(targetBytes), maxProcesses);
}