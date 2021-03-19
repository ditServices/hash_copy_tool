//
// Created by Howard Colin on 14/03/2021.
//

#ifndef HASH_COPY_HANDLER_H
#define HASH_COPY_HANDLER_H

#include <glib.h>
#include "source_handler.h"
/* Structure to hold our hashed files before copying */

typedef struct cp_hashed_file {
    const char *file_path;
    int filename;
    off_t size;
    u_int64_t hash_digest;
} cp_hashed_file;

typedef struct copy_handler {
    src_handler *source_files;
    char **destinations;
    GArray* hashed_file_paths;
} copy_handler;

copy_handler* new_copy_handler(char* src_path, char **destinations);

int check_destinations(char **destinations, unsigned int total_size);

int process_xxh(copy_handler *copy_job);

int process_copy(copy_handler *copy_job);

int verify(copy_handler *copy_job);

void close_copy(copy_handler *copy_job);
#endif //HASH_COPY_HANDLER_H
