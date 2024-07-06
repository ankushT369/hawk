#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <limits.h>

#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Initialize inotify
    int inotify_fd = inotify_init();
    if (inotify_fd == -1) {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }

    // Add watch for the specified file
    int watch_fd = inotify_add_watch(inotify_fd, argv[1], IN_MODIFY | IN_DELETE_SELF | IN_MOVE_SELF);
    if (watch_fd == -1) {
        perror("inotify_add_watch");
        close(inotify_fd);
        exit(EXIT_FAILURE);
    }

    printf("Watching %s for changes...\n", argv[1]);

    // Buffer to hold events
    char buf[BUF_LEN] __attribute__((aligned(8)));
    ssize_t num_read;
    char *ptr;
    struct inotify_event *event;
    int count = 0;

    while (1) {
        num_read = read(inotify_fd, buf, BUF_LEN);
        if (num_read == -1) {
            perror("read");
            close(watch_fd);
            close(inotify_fd);
            exit(EXIT_FAILURE);
        }

        printf("num_read = %ld\n", num_read);

        for (ptr = buf; ptr < buf + num_read; ) {
            printf("val = %d\n", count);
            count++;
            event = (struct inotify_event *) ptr;

            printf("Event mask: 0x%08x\n", event->mask);
            if (event->len > 0) {
                printf("Event name: %s\n", event->name);
            }

            printf("Event: ");
            if (event->mask & IN_MODIFY) printf("File %s modified\n", argv[1]);
            if (event->mask & IN_DELETE_SELF) printf("File %s deleted\n", argv[1]);
            if (event->mask & IN_MOVE_SELF) printf("File %s moved\n", argv[1]);

            ptr += sizeof(struct inotify_event) + event->len;
        }
    }

    // Cleanup (though this part of the code is never reached in this example)
    close(watch_fd);
    close(inotify_fd);

    exit(EXIT_SUCCESS);
}

/*
#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <limits.h>
#include <time.h>

#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))
#define SUPPRESSION_INTERVAL 0.3  // Interval in seconds to suppress duplicate events

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Initialize inotify
    int inotify_fd = inotify_init();
    if (inotify_fd == -1) {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }

    // Add watch for the specified file
    int watch_fd = inotify_add_watch(inotify_fd, argv[1], IN_MODIFY | IN_DELETE_SELF | IN_MOVE_SELF);
    if (watch_fd == -1) {
        perror("inotify_add_watch");
        close(inotify_fd);
        exit(EXIT_FAILURE);
    }

    printf("Watching %s for changes...\n", argv[1]);

    // Buffer to hold events
    char buf[BUF_LEN] __attribute__((aligned(8)));
    ssize_t num_read;
    char *ptr;
    struct inotify_event *event;
    time_t last_event_time = 0;

    while (1) {
        num_read = read(inotify_fd, buf, BUF_LEN);
        if (num_read == -1) {
            perror("read");
            close(watch_fd);
            close(inotify_fd);
            exit(EXIT_FAILURE);
        }

        printf("num_read = %ld\n", num_read);

        for (ptr = buf; ptr < buf + num_read; ) {
            event = (struct inotify_event *) ptr;

            time_t current_time = time(NULL);
            if (difftime(current_time, last_event_time) > SUPPRESSION_INTERVAL) {
                if (event->len > 0) {
                    printf("Event name: %s\n", event->name);
                }

                printf("Event: ");
                if (event->mask & IN_MODIFY) printf("File %s modified\n", argv[1]);
                if (event->mask & IN_DELETE_SELF) printf("File %s deleted\n", argv[1]);
                if (event->mask & IN_MOVE_SELF) printf("File %s moved\n", argv[1]);

                last_event_time = current_time;
            }

            ptr += sizeof(struct inotify_event) + event->len;
        }
    }

    // Cleanup (though this part of the code is never reached in this example)
    close(watch_fd);
    close(inotify_fd);

    exit(EXIT_SUCCESS);
}
*/
