#ifndef HAWK_H
#define HAWK_H

#if !defined(__linux__)
#error "This code requires a Linux environment."
#endif


#include <stdint.h>


/*
 *
 *
 */

typedef enum {
    USER,
    NOT_USER
} user_identify;


/*  enumerator contains :
 *
 *  INSTALL to detect the install part
 *
 *  UNEXPECTED to detected the rest 
 *
 */

typedef enum {
    INSTALL, 
    UNEXPCTED
} installation_request;



/*  struct definition : 
 *
 */

struct installation_chunk {
    /* recored the intial timestamp */
    char* initial_timestamp;        

    /* installation command should be copied and executed in the script */
    char* installation_command;

    /* check if its user */
    user_identify is_user;

    /* check installation request */
    installation_request installation_request;

    /* recored the final timestamp */
    char* final_timestamp;        
};



/* Struct : Package manager information
 *
 */
struct pm_info {
    const char* release_file;
    const char* package_manager;
};



/* Array of structs mapping release files to package managers */
struct pm_info info_inst [] = {
    {"/etc/redhat-release", "yum"},
    {"/etc/arch-release", "pacman"},
    {"/etc/gentoo-release", "emerge"},
    {"/etc/SuSE-release", "zypp"},
    {"/etc/debian_version", "apt"},
    {"/etc/alpine-release", "apk"},
};

extern const char* package;

const char* fetch_package_manager();






#endif // HAWK_H

