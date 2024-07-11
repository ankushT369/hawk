#include <sys/inotify.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdbool.h>

#include "hawk_monitor.h"
#include "hawk_log.h"
#include "hawk_parser.h"


/* Struct to hold the file and watch descriptors */
struct inotify_descriptors d_instance;

struct file_config fc_instance;

const uint32_t watch_mask = IN_MODIFY;


/* File monitoring gets initialised */
void monitoring_service_init() {
    /* */
    d_instance._fd = inotify_init();

    /* */
    d_instance._wd = inotify_add_watch(d_instance._fd, FILENAME, watch_mask);


    monitoring_event_handler(d_instance, FILENAME);
         
}


void monitoring_event_handler(struct inotify_descriptors, const char* _filename) {
    // Later make it control flag
    bool run = true;

    pr_msg("Monitoring Started...\n");

    char event_buffer[BUFFER_LEN] __attribute__((aligned(8)));              // buffer to hold the inotify event

    fc_instance.fd = open(FILENAME, O_RDONLY);

    while(run) {

        // Move to the end of the file
        fc_instance.curr_pos = lseek(fc_instance.fd, 0, SEEK_END);

        // Skiped error-handling for this
        size_t event_bytes = read(d_instance._fd, event_buffer, BUFFER_LEN);

        for(char* ptr = event_buffer; ptr < event_buffer + event_bytes; ) {
            struct inotify_event* event = (struct inotify_event*) ptr;

            if(event -> mask & IN_MODIFY) {
                //printf("File : %s modified\n", FILENAME); 
                fetch_data(fc_instance);
            }

            ptr += sizeof(struct inotify_event) + event -> len;
        }

    }

    close(d_instance._fd);
    close(d_instance._wd);

}

