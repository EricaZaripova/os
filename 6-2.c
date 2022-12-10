#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>


void get_time(char* into_string)
{
    time_t now_time;
    time(&now_time);
    struct tm* now_tm = localtime(&now_time);
    struct timespec now_timespec;
    clock_gettime(CLOCK_MONOTONIC, &now_timespec);
    long milliseconds = now_timespec.tv_nsec / 1000000;

    snprintf(into_string, SIZE, "%d:%d:%d:%ld", now_tm->tm_hour, now_tm->tm_min, now_tm->tm_sec, milliseconds);
}

void print_pids(int flag)
{
    pid_t pid = getpid();
    printf("%d", pid);
    char time_string[200];
    if (pid != flag)
    {
        get_time(time_string);
        printf("This is a child process pid: %d time: %s\n", getpid(), time_string);
        get_time(time_string);
        printf("This is a child process parent pid: %d\n time: %s\n\n", getppid(), time_string);
    }
    else
    {
        get_time(time_string);
        printf("This is a parent process pid: %d time: %s\n\n", getpid(), time_string);
    }
}

int main()
{
    int flag = getpid();
    pid_t pid1 = fork();

    if (pid1 > 0)
    {
        pid_t pid2 = fork();
      
        if (pid2 > 0)
        {
            system("ps -x");
        }
    }
    print_pids(flag);

    return 0;
}
