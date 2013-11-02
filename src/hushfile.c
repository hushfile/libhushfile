/* vim: set sw=4 sts=4 et foldmethod=syntax : */

/*
 * Copyright (c) 2013 Alexander Færøy
 * Copyright (c) 2013 Thomas Steen Rasmussen
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <glib.h>

#include <curl/curl.h>

#include "hushfile.h"

static void usage(const char *name)
{
    fprintf(stderr, "Usage: %s [-qmfhpc] [file | url]\n", name);
}

int main(int argc, char *argv[])
{
    // Cursor.
    int c;

    // Store argv[0].
    const char *program_name = argv[0];

    // Environment.
    Environment *env = initialize_environment();

    while (-1 != (c = getopt(argc, argv, "m:f:u:p:c:qh")))
    {
        switch (c)
        {
            // Quiet.
            case 'q':
                env->quiet = true;
                break;

            // Override MIME-type.
            case 'm':
                env->mime_type_override = optarg;
                break;

            // Override Filename.
            case 'f':
                env->filename_override = optarg;
                break;

            // Override Hushfile URL.
            case 'u':
                env->hushfile_url = optarg;
                break;

            // Override default randomly generated password.
            case 'p':
                env->password = optarg;
                break;

            // Override default configuration path.
            case 'c':
                if (env->config_path != NULL)
                    free(env->config_path);

                env->config_path = expand_tilde(optarg);
                break;

            // Display usage.
            default:
                usage(program_name);
                return EXIT_FAILURE;
        }
    }

    if (argc - optind < 1)
    {
        usage(program_name);
        return EXIT_FAILURE;
    }

    argc -= optind;
    argv += optind;

    const char *filename = argv[0];

    if (filename == NULL)
    {
        usage(program_name);
        return EXIT_FAILURE;
    }

    // Figure out if we are going to upload or download.
    bool upload = false;

    if (access(filename, F_OK) != -1)
    {
        // File exists and we are going to upload it.
        upload = true;
    }
    else
    {
        // File does not exist and we need to check if it's an URL.
        if (strprefix(filename, "https://"))
            upload = false;
        else if (strprefix(filename, "http://"))
        {
            fprintf(stderr, "Are you sure you didn't mean https:// ?\n");
            return EXIT_FAILURE;
        }
        else
        {
            usage(program_name);
            return EXIT_FAILURE;
        }
    }

    // Initialize Curl.
    curl_global_init(CURL_GLOBAL_DEFAULT);

    // Check if configuration directory exists, otherwise create it.
    char *config_dir_name = g_path_get_dirname(env->config_path);

    if (! g_file_test(config_dir_name, G_FILE_TEST_EXISTS))
    {
        // Create the config directory.
        if (g_mkdir(config_dir_name, 0500) == -1)
        {
            fprintf(stderr, "Error: Unable to create configuration directory %s: %s\n", config_dir_name, strerror(errno));
            free(config_dir_name);

            return EXIT_FAILURE;
        }

        fprintf(stderr, "Creating configuration directory ...\n");
    }

    free(config_dir_name);

    if (upload)
    {
    }
    else
    {
        download_file(env, "https://secure.0x90.dk/~ahf/6e2c371e29a08c3e24eaecf94dc5577f.tar.gz", "/tmp/lol");
    }

    // Cleanup.
    curl_global_cleanup();

    return EXIT_SUCCESS;
}
