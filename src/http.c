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

#include <assert.h>
#include <math.h>
#include <stdio.h>

#include <curl/curl.h>

#include <openssl/ssl.h>

#include "http.h"
#include "utilities.h"

#define PROGRESS_WIDTH 50

static int progress_callback(void *env_ptr, double downloaded_total, double downloaded_now, double uploaded_total, double uploaded_now)
{
    Environment *env = env_ptr;
    double now;
    double total;
    double fraction;

    if (downloaded_total > 0.0)
    {
        total = downloaded_total;
        now = downloaded_now;
    }
    else if (uploaded_total > 0.0)
    {
        total = uploaded_total;
        now = uploaded_total;
    }
    else
        return 0;

    fraction = now / total;
    int width = round(fraction * PROGRESS_WIDTH);
    int remaining_width = PROGRESS_WIDTH - width;

    hprintf(env, "%4.0f%% [", fraction * 100);

    for (int i = 0; i < width; i++)
        hprintf(env, "=");

    for (int i = 0; i < remaining_width; i++)
        hprintf(env, " ");

    hprintf(env, "]\r");
    fflush(stdout);

    return 0;
}

static int ssl_context_callback(CURL *curl, void *ssl_ctx_ptr, void *env_ptr)
{
    Environment *env = env_ptr;
    SSL_CTX *ssl_ctx = ssl_ctx_ptr;

    return CURLE_OK;
}

static size_t write_callback(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    return nmemb;
}

void download_file(Environment *env, const char *url, const char *destination_path)
{
    CURL *curl;
    CURLcode result = CURLE_OK;
    curl = curl_easy_init();
    assert(curl);

    // Initialize.
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);

    // Progress callback.
    curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress_callback);
    curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, env);

    // SSL options.
    curl_easy_setopt(curl, CURLOPT_SSL_CTX_FUNCTION, ssl_context_callback);
    curl_easy_setopt(curl, CURLOPT_SSL_CTX_DATA, env);

    // SS

    // Write callback.
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, NULL);

    result = curl_easy_perform(curl);

    if (result != CURLE_OK)
        hprintf(env, "Error: %s\n", curl_easy_strerror(result));

    hprintf(env, "\n");

    curl_easy_cleanup(curl);
}

void upload_file(Environment *env, const char *url, const char *source_path)
{
}
