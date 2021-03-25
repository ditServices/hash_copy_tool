//
// Created by howard on 14/03/2021.
//

#ifndef HASH_SOURCE_HANDLER_H
#define HASH_SOURCE_HANDLER_H

#include <glib.h>

/* Our src file object that will be loaded with file information.
 * Eventually this will be placed inside our src_handler queue.
 */

typedef struct file {
    off_t size;
    int filename;
    const char *file_path;
} file;

file* new_file(const char * src_filepath, off_t size, int filename);

/* src_handler Object acts as the base structure for the whole source directory.
 * It includes the whole directory size and a queue of
 * src_file structs to be processed
 */

typedef struct file_handler {
    GQueue* src_list;
    off_t src_size;
} file_handler;

file_handler* new_file_handler(char* src_path);

void delete_handler(file_handler *src_file_handler);

#endif //HASH_SOURCE_HANDLER_H
