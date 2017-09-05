/*
 * Simple libcurl-based GET client.
 *
 * From curl examples at:
 * https://curl.haxx.se/libcurl/c/simplepost.html
 */

/* 
 * <DESC>
 * Very simple HTTP GET
 * </DESC>
 */
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

int main(int argc, char *argv[])
{
    CURL *curl;
    CURLcode res;

    char *url;
    char *postthis;

    if (argc < 3) {
        fprintf(stderr, "Usage: %s url\n", argv[0]);
        return 1;
    }

    url = argv[1];
    postthis = argv[2];

    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "libcurl failed to initialize. Exiting\n");
        return 2;
    } 

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postthis);

    /* If we don't provide POSTFIELDSIZE, libcurl will strlen() by itself */
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long) strlen(postthis));

    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if (res != CURLE_OK)
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));

    /* always cleanup */
    curl_easy_cleanup(curl);

    return 0;
}
