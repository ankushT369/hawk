#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>  // Include for NAME_MAX

#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

void handle_events(int inotify_fd, const char *filename) {
    char buf[BUF_LEN] __attribute__((aligned(8)));

    while (1) {
        printf("fewf\n");
        ssize_t num_read = read(inotify_fd, buf, BUF_LEN);
        if (num_read == -1) {
            if (errno != EAGAIN) {
                perror("read");
            }
            continue;
        }

        for (char *ptr = buf; ptr < buf + num_read; ) {
            printf("susu\n");
            struct inotify_event *event = (struct inotify_event *) ptr;

            if (event->mask & IN_MODIFY) {
                printf("File %s modified\n", filename);

                // Asynchronously read the file content
                int file_fd = open(filename, O_RDONLY | O_NONBLOCK);
                if (file_fd == -1) {
                    perror("open");
                    continue;
                }

                char buffer[1024];
                ssize_t bytes_read;
                FILE *output_file = fopen("output.txt", "a"); // Open file in append mode
                if (!output_file) {
                    perror("output.txt");
                    close(file_fd);
                    continue;
                }

                while ((bytes_read = read(file_fd, buffer, sizeof(buffer) - 1)) > 0) {
                    buffer[bytes_read] = '\0';
                    fprintf(output_file, "%s", buffer); // Write to output.txt
                }

                if (bytes_read == -1) {
                    perror("read");
                }

                fclose(output_file);
                close(file_fd);
            }

            ptr += sizeof(struct inotify_event) + event->len;
        }
    }
}

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

    // Handle events asynchronously
    handle_events(inotify_fd, argv[1]);

    // Cleanup
    close(watch_fd);
    close(inotify_fd);

    exit(EXIT_SUCCESS);
}

