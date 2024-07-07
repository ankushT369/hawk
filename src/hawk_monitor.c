#include <sys/inotify.h>
#include <unistd.h>

#include "hawk_monitor.h"



/* Struct to hold the file and watch descriptors */
struct inotify_descriptors d_instance;



/* File monitoring gets initialised */
void hawk_monitoring_service_init() {
    /* */

    /* */
    d_instance._fd = inotify_init1();

    /* */
    d_instance._wd = inotify_add_watch(d_instance._fd, _filename, IN_MODIFY);
    

    hawk_monitoring_event_handler(d_instance, FILENAME);
         
}


void hawk_monitoring_event_handler(struct inotify_descriptors, const char* _filename) {
    bool run = true;

    char event_buffer[BUFFER_LEN] __attribute__((aligned(8)));              // buffer to hold the inotify event

    while(run) {
        // Skiped error-handling for this
        size_t event_bytes = read(d_instance._fd, event_buffer, BUFFER_LEN);

        for(char* ptr = event_buffer; ptr < event_buffer + event_buffer; ) {

            struct inotify_event* event = (struct inotify_event*)(ptr);

            if(event -> mask & IN_MODIFY) {
                printf("File : %s modified\n", FILENAME); 
            }

            ptr += sizeof(struct inotify_event) + event -> len;
        }

    }

}












