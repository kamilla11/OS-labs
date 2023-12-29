#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>

void printCurrentTime()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  long milliseconds = tv.tv_usec / 1000;
  printf("%02d:%02d:%02d:%03ld\n", (int)tv.tv_sec % 3600 / 60, (int)tv.tv_sec % 60, (int)tv.tv_sec / 3600, milliseconds);
}

int main()
{
  // Fork the first child process
  pid_t pid1 = fork();
  if (pid1 == 0)
  {
    // First child process
    // Print the process ID and parent process ID
    printf("First child process: PID %d, PPID %d\n", getpid(), getppid());
    // Print the current time
    printCurrentTime();
    return 0;
  }
  else if (pid1 < 0)
  {
    // Error forking the first child process
    printf("Error forking first child process.\n");
    return 1;
  }

  // Fork the second child process
  pid_t pid2 = fork();
  if (pid2 == 0)
  {
    // Second child process
    // Print the process ID and parent process ID
    printf("Second child process: PID %d, PPID %d\n", getpid(), getppid());
    // Print the current time
    printCurrentTime();
    return 0;
  }

  int status;

  // Executing the "ps -x" command in the parent process
  system("ps -x");

  // Waiting for the completion of both child processes
  waitpid(pid1, &status, 0);
  waitpid(pid2, &status, 0);

  printf("Parent process completed.\n");

  return 0;
}