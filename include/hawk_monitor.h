#ifndef HAWK_MONITOR_H
#define HAWK_MONITOR_H

/*  inotify will be used for the monitoring of the /var/log/apt/history.log
 *
 */

/* This header is used to monitor the changes */
#include <sys/inotify.h>




void hawk_monitoring_service_init();



#endif // HAWK_MONITOR_H
