#include <stddef.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hawk.h"
#include "hawk_monitor.h"
#include "hawk_log.h"

#define LINE_SIZE 256
#define MAX_PACKAGES 100 // Maximum number of unique packages
#define PACKAGE_NAME_SIZE 64 // Maximum package name length


struct pm_info pm_info_inst[] = {
    {"/etc/redhat-release", "yum"},
    {"/etc/arch-release", "pacman"},
    {"/etc/gentoo-release", "emerge"},
    {"/etc/SuSE-release", "zypp"},
    {"/etc/debian_version", "apt"},
    {"/etc/alpine-release", "apk"},
    {NULL, NULL}
};


// package set to NULL to avoid linking error
const char* package = NULL;


/* Function fetch the package manager */
void fetch_set_package_manager() {
    for(int i = 0; pm_info_inst[i].release_file != NULL; i++) {
        // access checks whether the path is valid
        if(access(pm_info_inst[i].release_file, F_OK) == 0) {
            package = pm_info_inst[i].package_manager;
        }
    }
}

void install() {
    fetch_set_package_manager();
    FILE *input_file = fopen("buffer.log", "r");

    if (!input_file) {
        perror("Error opening buffer.log");
    }

    char line[LINE_SIZE];
    char installed_packages[MAX_PACKAGES][PACKAGE_NAME_SIZE];
    int package_count = 0;

    while (fgets(line, sizeof(line), input_file)) {
        // Check if the line contains "Commandline: <package_manager> install"
        if (strstr(line, "Commandline:") && strstr(line, package) && strstr(line, "install")) {
            // Extract the package name
            char *token = strtok(line, " ");
            while (token) {
                // Look for the package name after "install"
                if (strcmp(token, "install") == 0) {
                    token = strtok(NULL, " "); // Get the next token (the package name)
                    if (token) {
                        // Remove any suffix (like :amd64) if present
                        char *colon_pos = strchr(token, ':');
                        if (colon_pos) {
                            *colon_pos = '\0'; // Null-terminate the string at ':'
                        }

                        // Check for duplicate packages
                        int is_duplicate = 0;
                        for (int i = 0; i < package_count; i++) {
                            if (strcmp(installed_packages[i], token) == 0) {
                                is_duplicate = 1;
                                break;
                            }
                        }

                        // If not a duplicate, add it to the list
                        if (!is_duplicate && package_count < MAX_PACKAGES) {
                            strncpy(installed_packages[package_count], token, PACKAGE_NAME_SIZE);
                            installed_packages[package_count][PACKAGE_NAME_SIZE - 1] = '\0'; // Ensure null-termination
                            package_count++;
                        }
                    }
                    break; 
                }
                token = strtok(NULL, " ");
            }
        }
    }

    fclose(input_file);

    // Run installation command for each unique package
    for (int i = 0; i < package_count; i++) {
        const char* packages = installed_packages[i]; // Use const char* for package
       
        char command[LINE_SIZE];
        snprintf(command, sizeof(command), "sudo %s install -y %s", package, packages);
        
        printf("Running command: %s\n", command);
        int status = system(command); // Execute the install command
        
        if (status == -1) {
            perror("Error executing install command");
        } else {
            printf("Installation status for %s: %d\n", package, WEXITSTATUS(status));
        }
    }

}

void handle_flag(int argc, char* argv[]) {
    if (argc == 1) {
        fetch_set_package_manager();
        monitoring_service_init();
    } 
    else if (argc == 2 && strcmp(argv[1], "-i") == 0) {
        pr_msg("Installation flag -i detected. Running installation function.\n");
        install();
    } 
    else {
        fprintf(stderr, "Error: Invalid flag or argument. Use '-i' for installation mode.\n");
    }
}

int main(int argc, char* argv[]) {
    handle_flag(argc, argv);
    return 0;
}
