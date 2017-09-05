/*
 * Simple libcurl-based GET client.
 *
 * From curl examples at:
 * https://curl.haxx.se/libcurl/c/https.html
 */

/* 
 * <DESC>
 * Very simple HTTP GET
 * </DESC>
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>

void usage(char *progname, int exitcode, char *msg)
{
    if (msg)
        fprintf(stderr, "%s\n", msg);

    fprintf(stderr, "Usage: %s [-s serverCA] host:port\n", progname);
    exit(exitcode);
}

int main(int argc, char *argv[])
{
    CURL *curl;
    CURLcode res;

    int opt;
    char *url;
    char *cafile = NULL;

    if (argc < 2)
        usage(argv[0], 1, NULL);

    while ((opt = getopt(argc, argv, "s:")) != -1) {
        switch (opt) {
        case 's':
           cafile = optarg;
           break;

        default: /* ? */
            usage(argv[0], 1, "Invalid option");
        }
    }

    if (optind >= argc) 
        usage(argv[0],  2, "Expected hostname[:port] argument");

    url = argv[optind];

    /* global initialization */
    if (curl_global_init(CURL_GLOBAL_ALL) != 0) {
        fprintf(stderr, "curl_global_init() failed. Exiting\n");
        return 3;
    }

    /* easy interface initialization */
    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "curl_easy_init() failed. Exiting\n");
        return 3;
    } 

#ifdef SKIP_PEER_VERIFICATION
    /* 
     * If you want to connect to a site who isn't using a certificate that
     * is signed by one of the certs in the CA bundle you have, you can
     * skip tge verification of the server's certificate. This makes 
     * the connection A LOT LESS SECURE.
     *
     * If you have a CA cert for the server stored someplace else than in the
     * default bundle, then the CURLOPT_CAPATH option might come in handy
     * for you.
     */
    set_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

#ifdef SKIP_HOSTNAME_VERIFICATION
    /*
     * If the site you're cvonnecting to uses a different host name that
     * what they have mentioned in their server certificate's commonName
     * (or subjectAltName) fields, libcurl will refuse to connect. You
     * can skip this check, but this will make the connection less secure.
     */
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif

    if (cafile)
        curl_easy_setopt(curl, CURLOPT_CAINFO, cafile);

    curl_easy_setopt(curl, CURLOPT_URL, url);
    /* Tell libcurl to follow redirection */
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if (res != CURLE_OK)
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));

    /* always cleanup */
    curl_easy_cleanup(curl);

    /* global cleanup */
    curl_global_cleanup();

    return 0;
}
