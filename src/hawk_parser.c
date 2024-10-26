#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <hawk_parser.h>

#define BUFFER_FILE_PATH "buffer.log"

// Function to log the command to the buffer file if it contains "install"
void parse_data(char *data) {  // Change const char* to char*
    char command[256];
    char start_date[64]; // Variable to hold the start date

    // Write data to buffer.log
    FILE *buffer_file = fopen(BUFFER_FILE_PATH, "a");
    if (buffer_file) {
        fprintf(buffer_file, "%s", data);
        fclose(buffer_file);
    } else {
        // perror("Could not open buffer.log");
        return;
    }

    // Parse command from the data string
    if (sscanf(data, "Start-Date: %[^\n]\nCommandline: %[^\n]", start_date, command) == 2) {
        // Check if the command contains "install"
        if (strstr(command, "install")) {
            //log_command(command); // Log the command to .hawk/db.log
        } else {
        
        }
    } else {

    }
}
// Function to handle fetched data
void parse_fetched_data(struct file_content file_info) {
    parse_data(file_info.data);
}

