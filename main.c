/* MAIN ENTRY POINT INTO HASH COPY TOOL
 *
 * Program runs with the following logic:
 *  - Parse command line args (src_args) (dest_args)
 *  - Scan src directory and populate src structure
 *      - Src structure contains a queue of files to process
 *      - The total size of the src folder
 *      - Checks are performed to ensure that the destination(s) to accept the size to be copied
 *  - Destination structure is initialized
 *  - Copy structure is initialized
 *      - Processes the queue from the src
 *          - Copying to destinations stored in destination structure.
 *
 */

#include <stdio.h>
#include <glib.h>

#include "vprint.h"
#include "copy_handler.h"

// Global argument variables
static char *src_args = NULL;
static char **dest_args = NULL;
static gboolean verbose_mode = FALSE;

// Argument parsing option context
static GOptionEntry entries[] = {
        { "verbose", 'v', 0, G_OPTION_ARG_NONE, &verbose_mode, "Verbose mode", NULL },
        {"input",        'i', G_OPTION_FLAG_NONE, G_OPTION_ARG_STRING,       &src_args,     "Input files",       NULL},
        {"destination", 'd', G_OPTION_FLAG_NONE, G_OPTION_ARG_STRING_ARRAY, &dest_args, "Copy destinations", NULL},
        {NULL}
};

int main(int argc, char **argv) {

    /*
     Here we init out argument parsing logic. Data is parsed from argv via glib and stored into
     the global variables (src_args) and (dest_args)
     */

    GOptionContext *option_context;
    GError *error = NULL;

    option_context = g_option_context_new("- Securely copy files from one destination to another with xxHash checksum.");
    g_option_context_add_main_entries(option_context, entries, NULL);

    if (!g_option_context_parse(option_context, &argc, &argv, &error)) {
        g_printerr("%s: %s\n", argv[0], error->message);
        return 1;
    }
    g_option_context_free(option_context);

    //Verbose mode (-v) status notification
    vprint(verbose_mode, "*** VERBOSE MODE ACTIVATED ***\n");

    copy_handler *copy_job = new_copy_handler(src_args, dest_args);

    vprint(verbose_mode,"Scanning Source: %s\n", src_args);
    vprint(verbose_mode,"Total copy size: %.2f GB\n", ((float) copy_job->source_files->src_size) / 1000 / 1000 / 1000); //Could be better...
    vprint(verbose_mode,"Total entries: %d\n", copy_job->source_files->src_list->length);
    process_xxh(copy_job);

    close_copy(copy_job);
    return 0;
}
