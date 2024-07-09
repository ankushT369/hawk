#ifndef HAWK_MONITOR_H
#define HAWK_MONITOR_H

/* inotify will be used for the monitoring of the /var/log/apt/history.log */

#include <limits.h>
#include <sys/inotify.h>
#include <stdbool.h>

struct inotify_descriptors {
    int _fd;
    int _wd;
};


/*  
 * The buffer given to read() should be at least
 * (sizeof(struct inotify_event) + NAME_MAX + 1) bytes, 
 * where NAME_MAX is the maximum length of a filename, 
 * plus one for the terminating null byte.
 *
*/

#define BUFFER_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

#define FILENAME "/var/log/apt/history.log"


void hawk_monitoring_service_init();
void hawk_monitoring_event_handler(struct inotify_descriptors, const char* _filename);



#endif // HAWK_MONITOR_H
