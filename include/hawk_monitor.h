#ifndef HAWK_MONITOR_H
#define HAWK_MONITOR_H

/*  inotify will be used for the monitoring of the /var/log/apt/history.log
 *
 */

//#define FILEPATH /var/log/apt/history.log

/* necessary headers included */
#include <sys/inotify.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>

/*  
 * The buffer given to read() should be at least
 * 
 * (sizeof(struct inotify_event) + NAME_MAX + 1) bytes, 
 *
 * where NAME_MAX is the maximum length of a filename, 
 *
 * plus one for the terminating null byte.
 *
*/

#define BUFFER_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))


char event_buffer[BUFFER_LEN] __attribute__((aligned(8)));              // buffer to hold the inotify event


void hawk_monitoring_service_init(char* filename);
void hawk_monitoring_event_handler();


#endif // HAWK_MONITOR_H
