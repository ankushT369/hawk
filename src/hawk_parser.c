#include <unistd.h>
#include <stdio.h>


#include "hawk_parser.h"



void fetch_data(struct file_config fc_instance) {

    fc_instance.curr_pos = lseek(fc_instance.fd, fc_instance.curr_pos, SEEK_SET);

    while((fc_instance.bytes_read = read(fc_instance.fd, fc_instance.buffer, sizeof(fc_instance.buffer) - 1)) > 0) {
        fc_instance.buffer[fc_instance.bytes_read] = '\0';
        printf("%s", fc_instance.buffer);
    }

    fc_instance.curr_pos = lseek(fc_instance.fd, 0, SEEK_CUR);

    //close(fc_instance.fd);
}
