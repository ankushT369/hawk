#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/inotify.h>
#include <pthread.h>
#include <errno.h>

#define EVENT_SIZE (sizeof(struct inotify_event))
#define EVENT_BUF_LEN (1024 * (EVENT_SIZE + 16))
#define BUFFER_SIZE 1024

void *read_file(void *arg) {
    FILE *input = fopen("input.txt", "r");
    FILE *output = fopen("output.txt", "a");
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    if (input == NULL || output == NULL) {
        perror("fopen");
        return NULL;
    }

    fseek(input, 0, SEEK_END);
    long last_pos = ftell(input);
    fseek(input, last_pos, SEEK_SET);

    while ((read = getline(&line, &len, input)) != -1) {
        fwrite(line, sizeof(char), read, output);
    }

    fclose(input);
    fclose(output);
    if (line) {
        free(line);
    }

    return NULL;
}

void *monitor_file(void *arg) {
    int length, i = 0;
    int fd;
    int wd;
    char buffer[EVENT_BUF_LEN];

    fd = inotify_init();
    if (fd < 0) {
        perror("inotify_init");
        return NULL;
    }

    wd = inotify_add_watch(fd, "input.txt", IN_MODIFY);

    while (1) {
        length = read(fd, buffer, EVENT_BUF_LEN);
        if (length < 0) {
            perror("read");
        }

        while (i < length) {
            struct inotify_event *event = (struct inotify_event *)&buffer[i];
            if (event->len) {
                if (event->mask & IN_MODIFY) {
                    printf("The file %s was modified.\n", event->name);
                    // Trigger asynchronous read and write
                    pthread_t read_thread;
                    pthread_create(&read_thread, NULL, read_file, NULL);
                }
            }
            i += EVENT_SIZE + event->len;
        }
    }

    inotify_rm_watch(fd, wd);
    close(fd);

    return NULL;
}


int main() {
    pthread_t monitor_thread;

    // Start monitoring the file in a separate thread
    pthread_create(&monitor_thread, NULL, monitor_file, NULL);

    // Keep the main thread running to allow monitoring
    pthread_join(monitor_thread, NULL);

    return 0;
}

