#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <readline/readline.h>
#include <string.h>
#include <sys/wait.h>

char **get_input(char *);
void sigint_handler (int);

int main(int argc, char *argv[] ) {
    char **command;
    char *input;
    pid_t child_pid;
    int stat_loc;
    char exit_command[] = "exit";
    char fname[30];
    int p[2];
    int fd;

    pipe(p);

   if( argc == 2 ) {
      printf("The argument supplied for the log file is %s\n", argv[1]);
      printf("The logfile named %s is being created...\n", argv[1]);
      fd = open(argv[1], O_CREAT | O_RDWR | O_APPEND | O_TRUNC, S_IRUSR | S_IWUSR);
   } else if( argc > 2 ) {
      printf("Too many arguments supplied. Please provide a string for the logfile\n");
       return 1;
   } else {
      printf("One argument expected. Please provide a string for the logfile\n");
       return 1;
   }

    while (1) {
        input = readline("loggingShell> ");
        command = get_input(input);

        if (!command[0]) {
            free(input);
            free(command);
            continue;
        }

        child_pid = fork();
    	if (child_pid > 0 && strcmp(input, exit_command)!= 0){
    	     printf("Child  %d executes %s  \n", child_pid, *command);
             write(p[1], *command, 5);
             wait (NULL);
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

        read(p[0], buf, 5);

        printf("Child  %d completed with exit code 0  \n", child_pid);
        free(input);
        free(command);
    }
    return 0;
}

char **get_input(char *input) {
    char **command = malloc(8 * sizeof(char *));
    char *separator = " ";
    char *parsed;
    int index = 0;
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

void sigint_handler (int sig)
{
   printf("killing process %d\n",getpid());
   //exit(0);
}


