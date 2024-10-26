#ifndef HAWK_MONITOR_H
#define HAWK_MONITOR_H

/* inotify will be used for the monitoring of the /var/log/<package-manager>/history.log */
#include <limits.h>
#include <sys/inotify.h>
#include <sys/types.h>
#include <stdbool.h>


struct inotify_descriptors {
    int _fd;
    int _wd;
};


struct file_config {
    /* File descriptor of monitoring file */
    int fd;

    char buffer[4096];
    size_t bytes_read;
    off_t curr_pos;
    int whence;
};


struct file_content {
    char* data;
    size_t size;
};

/*  
 * The buffer given to read() should be at least
 * (sizeof(struct inotify_event) + NAME_MAX + 1) bytes, 
 * where NAME_MAX is the maximum length of a filename, 
 * plus one for the terminating null byte.
 *
*/

#define BUFFER_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))


void monitoring_service_init();
void monitoring_event_handler(struct inotify_descriptors, const char* _filename);
void fetch_data(struct file_config);



#endif // HAWK_MONITOR_H
