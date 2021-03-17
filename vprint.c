//
// Created by howard on 14/03/2021.
//

#include "vprint.h"

void vprint(int status, const char *fmt, ...) {
    if(status == 1) {
        va_list argp;
        va_start(argp, fmt);
        vprintf(fmt, argp);
        va_end(argp);
    }
}

#include "vprint.h"
