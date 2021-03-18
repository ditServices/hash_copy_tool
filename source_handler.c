//
// Created by howard on 14/03/2021.
//
#define _XOPEN_SOURCE 500

#include "source_handler.h"
#include <ftw.h>

// We need globals to work with nftw...
static off_t sh_total;
GQueue* sh_file_list;

/* return a pointer to a struct containing the src_file information.
 * This is called within the nftw callback function, it is needed to populate the
 * global sh_file_list queue.
 */

src_file* new_src_file(const char * src_filepath, off_t size, int filename) {
    src_file *sh_src_file = malloc(sizeof(src_file));
    sh_src_file->file_path = g_strdup(src_filepath);
    sh_src_file->size = size;
    sh_src_file->filename = filename;
    return sh_src_file;
}

void free_file_list(src_file *src_file_to_free) {
    free((char *) src_file_to_free->file_path);
    free(src_file_to_free);
}

/* Callback function for nftw. Data is populated in our sh_src_file struct and added
 * to the global sh_file_list queue. A running data total is also calculated.
 */

int process(const char *fpath, const struct stat *statptr, int flags, struct FTW *pfwt) {
    if(flags == FTW_F) {
        if(strcmp(fpath + pfwt->base, ".DS_Store") != 0) {
            src_file *sh_src_file = new_src_file(fpath, statptr->st_size, pfwt->base);
            g_queue_push_tail(sh_file_list, sh_src_file);
            sh_total += sh_src_file->size;
        }
    }
    return 0;
}

/* The main init for our src_handler object. This function returns a src_handler populated
 * with a queue of src files as well as the total data size. The global queue is freed after
 * it has been copied as it is no longer needed.
 */

src_handler* new_src_handler(char* src_path) {
    sh_total = 0;
    src_handler *sources = malloc(sizeof(src_handler));
    sh_file_list = g_queue_new();
    int fd_limit = 5;
    int flags = FTW_CHDIR | FTW_DEPTH | FTW_MOUNT | FTW_PHYS;
    int ret = nftw(src_path, process, fd_limit, flags);
    sources->src_size = sh_total;
    sources->src_list = g_queue_copy(sh_file_list);
    return sources;
}

//cleanup
void delete_handler(src_handler *src_file_handler) {
    g_queue_free_full(src_file_handler->src_list, (gpointer) free_file_list);
    g_queue_free_full(sh_file_list, (gpointer) free_file_list);
    free(src_file_handler);
}
