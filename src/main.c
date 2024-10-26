#include <stddef.h>
#include <unistd.h>

#include "hawk.h"
#include "hawk_monitor.h"
#include "hawk_log.h"


struct pm_info pm_info_inst[] = {
    {"/etc/redhat-release", "yum"},
    {"/etc/arch-release", "pacman"},
    {"/etc/gentoo-release", "emerge"},
    {"/etc/SuSE-release", "zypp"},
    {"/etc/debian_version", "apt"},
    {"/etc/alpine-release", "apk"},
    {NULL, NULL}
};


// package set to NULL to avoid linking error
const char* package = NULL;


/* Function fetch the package manager */
void fetch_set_package_manager() {
    for(int i = 0; pm_info_inst[i].release_file != NULL; i++) {
        // access checks whether the path is valid
        if(access(pm_info_inst[i].release_file, F_OK) == 0) {
            package = pm_info_inst[i].package_manager;
        }
    }
}



int main() {
    fetch_set_package_manager();
    monitoring_service_init();
    return 0;
}
