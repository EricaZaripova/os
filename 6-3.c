#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>


bool handle_command(char** argv)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        pid_t inner_pid = fork();
        if (inner_pid == 0)
        {
            execvp(argv[0], argv);
            printf("Error with find command %s\n", argv[0]);
        }
        else if (inner_pid > 0)
        {
            int status;
            waitpid(inner_pid, &status, 0);
            if (WIFEXITED(status))
            {   
                printf("pid: %d\n", getpid());
                int exit_status = WEXITSTATUS(status);
                if (exit_status != 0)
                {
                    printf("Program exited with code: %d\n", exit_status);
                }
            }
            else
            {
                printf("Unexpected program termination\n");
            }
        }
        else if (inner_pid < 0)
        {
            printf("Error with call fork\n");
        }
         return false;
    }
    else if (pid < 0)
    {
        printf("Error with create a new fork\nPlease try again\n");
    }
 return true;
}

int main()
{
    bool should_continue = true;
    while (should_continue)
    {
        char command[200];
        fgets(command, 200, stdin);
        
        char* argv[100];
        char* part = strtok(command, " \n");

        int i;
        for (i = 0; part != NULL; i++)
        {
            argv[i] = part;
            part = strtok(NULL, " \n");
        }
        argv[i] = NULL;


        should_continue = handle_command(argv);
    }
    return 0;
}
