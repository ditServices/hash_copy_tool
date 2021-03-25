//
// Created by Howard Colin on 14/03/2021.
//

#include "copy_handler.h"
#include "vprint.h"
#include <xxhash.h>
#include <stdio.h>
#include <sys/statvfs.h>

/* Check our destinations to make sure that they have enough space to accmodate
 * our newly copied files. Returns -1 on success, positive number relates to the index
 * of the destination array if there is not enough space
 */

int check_destinations(char **destinations, unsigned int total_size) {
    for(int i = 0; destinations[i]; i++){
        struct statvfs vfs;
        statvfs(destinations[i], &vfs);
        unsigned long free_space = vfs.f_bsize * vfs.f_bfree;
        if(total_size > free_space) return i;
    }
    return -1;
}

/* Create a new copy handler. We init the source file list and proceed to check
 * that our destinations have enough space for the copy
 * TODO: Check that destination paths exist.
 */

copy_handler* new_copy_handler(char* src_path, char **destinations) {
    copy_handler* copy_handler_init = malloc(sizeof(copy_handler));
    copy_handler_init->source_files = new_file_handler(src_path);
    copy_handler_init->destinations = destinations;
    int dest_status = check_destinations(copy_handler_init->destinations, copy_handler_init->source_files->src_size);
    if(dest_status != -1) {
        fprintf(stderr, "ERROR: Not enough space on %s\n*** EXITING ***\n", copy_handler_init->destinations[dest_status]);
        exit(-1);
    }
    copy_handler_init->hashed_file_paths = g_array_new(FALSE, FALSE, sizeof(cp_hashed_file));
    return copy_handler_init;
}

/* Here we process our file queue and populate the cp_hashed_file structure.
 * As we read the source file, we update the hash value in the structure, whilst this
 * happens we also iterate over the destinations and write to each
 */

uint64_t generate_xxh(const char *filename) {
    size_t const bufferSize = 512;
    void * const buffer = malloc(bufferSize);

    if(buffer == NULL) {
        fprintf(stderr, "Memory Error\n");
        return 0;
    }

    XXH64_state_t* const state = XXH64_createState();
    XXH64_hash_t const seed = 0;
    if(XXH64_reset(state, seed)) {
        free(buffer);
        return 0;
    }

    FILE *src = fopen(filename, "rb");
    if(!src) {
        free(buffer);
        fprintf(stderr, "ERROR - Could not open file\n");
        return 0;
    }

    ssize_t bytes;
    while((bytes = fread(buffer, 1, bufferSize, src)) != 0) {
        if(bytes == 0) {
            free(buffer);
            return 0;
        }
        if(XXH64_update(state, buffer, bytes) == XXH_ERROR) {
            free(buffer);
            return 0;
        }
    }

    uint64_t digest = XXH64_digest(state);
    fclose(src);
    XXH64_freeState(state);
    free(buffer);
    return digest;
}

/* Iterate over the source files and generate the hash. Once completed,
 * data is copied from the source file queue to the cp hashed file structure
 * which includes the files digest.
 */

int process_xxh(copy_handler *copy_job) {
    cp_hashed_file processed_file;
    int src_list_length = copy_job->source_files->src_list->length;
    file *src = g_queue_pop_head(copy_job->source_files->src_list);

    for(int i = 0; i <src_list_length; i++) {
        uint64_t digest = generate_xxh(src->file_path);
        processed_file.file_path = src->file_path;
        processed_file.filename = src->filename;
        processed_file.size = src->size;
        processed_file.hash_digest = digest;
        printf("Processed File: %s:\t %016llx\n", processed_file.file_path + processed_file.filename, processed_file.hash_digest);
        g_array_append_val(copy_job->hashed_file_paths, processed_file);
        src = g_queue_pop_head(copy_job->source_files->src_list);
    }
    return 0;
}

/* Reads the processed file array and copies each file to the array of destinations.
 * Directories are created inline.
 */

int process_copy(copy_handler *copy_job) {
    return 0;
}

/* Loops through each destination and generates a new set of hashes.
 * Hashes are checked against the source hashes stored in our hashed_file_paths structure.
 */

int verify(copy_handler *copy_job) {
    return 0;
}

// Clean up
void close_copy(copy_handler *copy_job) {
    delete_handler(copy_job->source_files);
    g_array_free(copy_job->hashed_file_paths, TRUE);
    free(copy_job);
}