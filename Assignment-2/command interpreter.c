#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGUMENTS 32

// Function to display the command prompt ($)
void display_prompt() {
    printf("$ ");
    fflush(stdout);
}

// Function to split a command line into arguments
void parse_command(char *command, char **arguments) {
    int i = 0;
    char *token = strtok(command, " ");
    
    while (token != NULL) {
        arguments[i++] = token;
        token = strtok(NULL, " ");
    }
    arguments[i] = NULL; // Null-terminate the argument list
}

int main() {
    char command[MAX_COMMAND_LENGTH];
    char *arguments[MAX_ARGUMENTS];
    int background = 0;
    
    while (1) {
        // Display the command prompt
        display_prompt();
        
        // Read the user input
        if (fgets(command, sizeof(command), stdin) == NULL) {
            perror("Failed to read input");
            exit(1);
        }
        
        // Remove the newline character from the input
        command[strlen(command) - 1] = '\0';
        
        // Check if the user wants to exit
        if (strcmp(command, "exit") == 0) {
            printf("Goodbye!\n");
            break; // Exit the loop and the program
        }
        
        // Check for background execution
        if (command[strlen(command) - 1] == '&') {
            background = 1;
            command[strlen(command) - 1] = '\0';
        } else {
            background = 0;
        }

        // Parse the command into arguments
        parse_command(command, arguments);

        // Fork a child process
        pid_t pid = fork();

        if (pid < 0) {
            perror("Fork failed");
            exit(1);
        } else if (pid == 0) {
            // This is the child process
            // Execute the user's command
            execvp(arguments[0], arguments);
            
            // If execvp fails, report an error
            perror("Command execution failed");
            exit(1);
        } else {
            // This is the parent process
            // If not background execution, wait for the child to complete
            if (!background) {
                int status;
                waitpid(pid, &status, 0);
            }
        }
    }

    return 0;
}
