#include <stdio.h>
#include <stdarg.h>
#include "hawk_log.h"

/* Print on level implementation */
void print_on_level(unsigned int loglevel, const char *format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
}

