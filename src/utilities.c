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

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glib.h>

#include "utilities.h"

bool strprefix(const char *a, const char *b)
{
    return strncmp(a, b, strlen(b)) == 0;
}

int hprintf(Environment *env, const char * restrict format, ...)
{
    if (env->quiet)
        return 0;

    int r;
    va_list arguments;

    va_start(arguments, format);
    r = vprintf(format, arguments);
    va_end(arguments);

    return r;
}

char *expand_tilde(const char *f)
{
    if (! f)
        return NULL;

    if (f[0] != '~')
        return g_strdup(f);

    return g_strconcat(g_get_home_dir(), f + 1, NULL);
}

HushfileIdentifier *parse_identifier(const char *url)
{
    // Format: https://xxx/[a-zA-Z0-9]#KEY
    char *identifier = alloca(100 * sizeof(char));
    char *key = alloca(100 * sizeof(char));

    if (sscanf(url, "https://%*[^/]/%99[^#]#%99s", identifier, key) != 2)
        return NULL;

    HushfileIdentifier *x = malloc(sizeof(HushfileIdentifier));
    x->identifier = strdup(identifier);
    x->key = strdup(key);

    memset(identifier, 0, 100);
    memset(key, 0, 100);

    return x;
}

void free_identifier(HushfileIdentifier *identifier)
{
    if (identifier == NULL)
        return;

    if (identifier->identifier != NULL)
        free(identifier->identifier);

    if (identifier->key != NULL)
        free(identifier->key);

    free(identifier);
}
