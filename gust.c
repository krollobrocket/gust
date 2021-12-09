#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#define DEFAULT_INTERVAL 500
#define DEFAULT_TIMEOUT_MS 5000

#define str(x) #x
#define xstr(x) str(x)

enum VERBS {
    GET,
    DELETE,
    OPTIONS,
    PATCH,
    POST,
    PUT
};

void help() {
  printf("Syntax: gust [-n COUNT] [-i INTERVAL] [-v] URL\n");
  printf("-n COUNT\tNumber of times to test URL\n");
  printf("-i INTERVAL\tTime between calls\n");
  printf("-x VERB\tHttp method\n");
  printf("-v\t\tDisplay verbose output\n");
}

size_t read_header(void *ptr, size_t size, size_t nmemb, void *stream) {
    printf("%s", (char*)ptr);
    return nmemb * size;
}

int valid_url(const char* url) {
    CURLU* u = curl_url();
    CURLUcode uc = curl_url_set(u, CURLUPART_URL, url, 0);
    curl_url_cleanup(u);
    return (uc == CURLUE_OK);
}

int main(int argc, char** argv) {
    CURL* ch;
    CURLcode code;
    CURLINFO info;
    int opt = 0;
    int verbose = 0;
    int head = 0;
    int count = -1;
    int interval = DEFAULT_INTERVAL;
    char* method = "GET";
    char* url = "";
    char* data = 0;

    while((opt = getopt(argc, argv, "n:i:x:vh")) != -1)
    {
        switch(opt)
        {
            case 'h':
                head = 1;
                break;
            case 'n':
                count = atoi(optarg);
                if (count == 0) {
                  count = -1;
                }
                break;
            case 'x':
                // Check so this is a valid verb.
                method = optarg;
                break;
            case 'i':
                interval = atoi(optarg);
                break;
            case 'v':
                verbose = 1;
                break;
            case '?':
                printf("Unknown option: %c\n", optopt);
                help();
                exit(EXIT_FAILURE);
                break;
        }
    }

    if (optind >= argc) {
        help();
        exit(EXIT_FAILURE);
    }

    // Check so we have a valid url
    url = argv[optind];
    if (!valid_url(url)) {
        printf("Invalid URL: %s\n", url);
        exit(EXIT_FAILURE);
    }

    ch = curl_easy_init();
    curl_easy_setopt(ch, CURLOPT_URL, url);
    curl_easy_setopt(ch, CURLOPT_VERBOSE, verbose);
    curl_easy_setopt(ch, CURLOPT_TIMEOUT_MS, DEFAULT_TIMEOUT_MS);
    if (strcmp(xstr(POST), method) == 0) {
        curl_easy_setopt(ch, CURLOPT_POST, 1);
    }
    if (strcmp(xstr(HEAD), method) == 0) {
        curl_easy_setopt(ch, CURLOPT_NOBODY, 1);
        curl_easy_setopt(ch, CURLOPT_HEADER, 1);
    }

    do {
        code = curl_easy_perform(ch);
        if (code != CURLE_OK) {
            break;
        }
        count--;
        usleep(interval * 1000);
    } while(count);

    curl_easy_cleanup(ch);

    return 0;
}
