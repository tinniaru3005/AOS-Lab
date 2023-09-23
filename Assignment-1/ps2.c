#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>

// Function to check if a string contains only digits
int isNumeric(const char *str) {
    while (*str) {
        if (!isdigit(*str))
            return 0;
        str++;
    }
    return 1;
}

// Function to list processes
void listProcesses(int showAll, int showAllUsers, const char *username) {
    DIR *dir;
    struct dirent *ent;
    struct passwd *pw;
    
    dir = opendir("/proc");
    
    if (dir != NULL) {
        printf("%5s %5s %10s %s\n", "PID", "PPID", "USER", "CMD");
        
        while ((ent = readdir(dir)) != NULL) {
            if (isNumeric(ent->d_name)) {
                char procPath[256];
                char cmdLine[256];
                char user[256];
                pid_t pid = atoi(ent->d_name);
                
                // Construct the path to the cmdline file for the process
                snprintf(procPath, sizeof(procPath), "/proc/%s/cmdline", ent->d_name);
                FILE *cmdFile = fopen(procPath, "r");
                
                if (cmdFile != NULL) {
                    if (fgets(cmdLine, sizeof(cmdLine), cmdFile) != NULL) {
                        cmdLine[strlen(cmdLine) - 1] = '\0';  // Remove newline character
                        
                        // Get the username of the process owner
                        if (showAllUsers) {
                            pw = getpwuid(getuid());
                            strncpy(user, pw->pw_name, sizeof(user));
                        } else {
                            pw = getpwuid(pid);
                            strncpy(user, pw->pw_name, sizeof(user));
                        }
                        
                        // Print process information based on options
                        if (showAll || (username != NULL && strcmp(user, username) == 0)) {
                            printf("%5s %5d %10s %s\n", ent->d_name, pid, user, cmdLine);
                        }
                    }
                    fclose(cmdFile);
                }
            }
        }
        closedir(dir);
    }
}

int main(int argc, char *argv[]) {
    int showAll = 0;
    int showAllUsers = 0;
    const char *username = NULL;
    
    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-a") == 0) {
            showAll = 1;
        } else if (strcmp(argv[i], "-ae") == 0) {
            showAll = 1;
            showAllUsers = 1;
        } else if (strcmp(argv[i], "-u") == 0 && i + 1 < argc) {
            username = argv[i + 1];
            i++;  // Skip the username argument
        }
    }
    
    listProcesses(showAll, showAllUsers, username);
    
    return 0;
}

