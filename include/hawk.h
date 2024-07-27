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
    PURGE,
    REMOVE,
    UNEXPCTED
} installation_request;



/*  struct definition : 
 *
 */

struct installation_chunk {
    /* installation command should be copied and executed in the script */
    char* installation_command;

    /* check if its user */
    user_identify is_user;

    /* check installation request */
    installation_request installation_request;

};



/* Struct : Package manager information
 *
 */
struct pm_info {
    const char* release_file;
    const char* package_manager;
};


/* */
extern struct pm_info pm_info_inst[]; 

/* Array of structs mapping release files to package managers */
/*
struct pm_info pm_info_inst [] = {
    {"/etc/redhat-release", "yum"},
    {"/etc/arch-release", "pacman"},
    {"/etc/gentoo-release", "emerge"},
    {"/etc/SuSE-release", "zypp"},
    {"/etc/debian_version", "apt"},
    {"/etc/alpine-release", "apk"},
    {NULL, NULL}
};
*/

extern const char* monitorable_file;

extern const char* package;

void fetch_set_package_manager();






#endif // HAWK_H

