#ifndef HAWK_H
#define HAWK_H

#if !defined(__linux__)
#error "This code requires a Linux environment."
#endif


#include <stdint.h>
#include <time.h>           // For struct tm


/*
 *
 *
 */

typedef enum {
    USER,
    NOT_USER
} hawk_user_request;


/*  enumerator contains :
 *
 *  INSTALL to detect the install part
 *
 *  UPGRADE to record the upgrade part
 *
 *  PURGE to record the remove part
 *
 *  REMOVE to record the purge part
 *
 *  UNEXPECTED to detected the rest 
 *
 */

typedef enum {
    INSTALL, 
    UPGRADE,
    REMOVE,
    PURGE,
    UNEXPECTED
} hawk_installation_request;



/*  struct definition : 
 *
 *
 *
 *
 *
 *
 */

struct hawk_installation_chunk {
    /* recored the intial timestamp */
    struct tm initial_timestamp;        

    /* installation command should be copied and executed in the script */
    char* installation_command;

    /* */
    hawk_user_request is_user;

    /* */
    hawk_installation_request installation_request;

    /* recored the final timestamp */
    struct tm final_timestamp;        
};




#endif // HAWK_H

