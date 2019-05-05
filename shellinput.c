#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <unistd.h>
#include <sys/wait.h>

char **get_input(char *);

int main() {
    char **command;
    char *input;
    pid_t child_pid;
    int stat_loc;
    char exit_command[] = "exit";


    while (1) {
        input = readline("loggingShell> ");
        command = get_input(input);

        if (!command[0]) {
            free(input);
            free(command);
            continue;
        }

        child_pid = fork();
    	if (child_pid != 0 && strcmp(input, exit_command)!=0){
    	     printf("Child  %d executes %s  \n", child_pid, *command);
    	} else {
            printf("exiting....");
        }

        if (child_pid == 0) {
            if (strcmp(input, exit_command) == 0) {
                int parent = getppid();
                kill(parent, SIGKILL);
                return 0;
        }

            execvp(command[0], command);
        } else {
            waitpid(child_pid, &stat_loc, WUNTRACED);
        }
        printf("Child  %d completed with exit code 0  \n", child_pid);
        free(input);
        free(command);
    }

    printf ("Exiting...");

    return 0;
}

char **get_input(char *input) {
    char **command = malloc(8 * sizeof(char *));
    char *separator = " ";
    char *parsed;
    int index = 0;
    char exit_command[] = "exit";
    char *str_command;

    parsed = strtok(input, separator);
    while (parsed != NULL) {
        command[index] = parsed;
        index++;

        parsed = strtok(NULL, separator);
    }

    command[index] = NULL;
    return command;
}


