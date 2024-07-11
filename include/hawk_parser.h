#ifndef HAWK_PARSER_H
#define HAWK_PARSER_H

#include <sys/types.h>




struct file_config {
    /* File descriptor of monitoring file */
    int fd;

    /* */
    char buffer[4096];
    
    /* */
    size_t bytes_read;

    /* */
    off_t curr_pos;

    /* */
    int whence;
};





// This function fetch data
void fetch_data(struct file_config);

 
// This function initiates the parser 
void parse_installation_data();



#endif // HAWK_PARSER_H
