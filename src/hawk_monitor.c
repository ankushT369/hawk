#include <sys/inotify.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdbool.h>

#include "hawk_monitor.h"
#include "hawk_log.h"
#include "hawk.h"


/* Struct to hold the file configuration */
struct file_config fc_instance;

/* Struct to hold the file information */
struct file_content info;

/* Watch mask to control the monitoring behaviour */
const uint32_t watch_mask = IN_CLOSE_WRITE;



/* File monitoring gets initialised */
void monitoring_service_init() {
    //pr_info("pacakage name : %s\n", package);
    struct inotify_descriptors d_instance;
    /* Initialize the inotify */
    d_instance._fd = inotify_init();

    char path[] = "/var/log/";
    char file[] = "history.log";

    size_t total_path_len = strlen(package) + strlen(path) + strlen(file) + 1; // Calculate total length of concatenated string (+1 for null terminator)

    char *filename = (char *)malloc(total_path_len);

    sprintf((char *)filename, "%s%s/%s", path, package, file);                 // Construct the concatenated path

    pr_info("Concatenated path: %s\n", filename);

    /* */
    d_instance._wd = inotify_add_watch(d_instance._fd, filename, watch_mask);

    monitoring_event_handler(d_instance, filename);
         
    free(filename);     // deallocating filename
}


void monitoring_event_handler(struct inotify_descriptors d_instance, const char* _filename) {
    // Later make it control flag
    bool run = true;

    pr_msg("Monitoring Started...\n");

    char event_buffer[BUFFER_LEN] __attribute__((aligned(8)));                 // buffer to hold the inotify event

    fc_instance.fd = open(_filename, O_RDONLY);

    while(run) {
        fc_instance.curr_pos = lseek(fc_instance.fd, 0, SEEK_END);             // Move to the end of the file
        
        size_t event_bytes = read(d_instance._fd, event_buffer, BUFFER_LEN);   // Skiped error-handling for this

        for(char* ptr = event_buffer; ptr < event_buffer + event_bytes; ) {
            struct inotify_event* event = (struct inotify_event*) ptr;

            if(event -> mask & watch_mask) {
                //printf("File : %s modified\n", _filename); 
                fetch_data(fc_instance);
            }

            ptr += sizeof(struct inotify_event) + event -> len;
        }

    }
}

void fetch_data(struct file_config fc_instance) {

    fc_instance.curr_pos = lseek(fc_instance.fd, fc_instance.curr_pos, SEEK_SET);

    while((fc_instance.bytes_read = read(fc_instance.fd, fc_instance.buffer, sizeof(fc_instance.buffer) - 1)) > 0) {
        fc_instance.buffer[fc_instance.bytes_read] = '\0';

        info.size += fc_instance.bytes_read + 1;                               // +1 for the null terminator
        info.data = realloc(info.data, info.size);

        //pr_info("%s", fc_instance.buffer);
        if (info.size == fc_instance.bytes_read + 1) {
            strcpy(info.data, fc_instance.buffer);                             
        } else {
            strcat(info.data, fc_instance.buffer);
        }
        pr_info("data : %s\n", info.data);
    }

    fc_instance.curr_pos = lseek(fc_instance.fd, 0, SEEK_CUR);

    /* thread implementation to parse content */

}

