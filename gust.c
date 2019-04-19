#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

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

int main(int argc, char** argv) {
    int opt = 0;
    int verbose = 0;
    int head = 0;
    int count = -1;
    int interval = 0;
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
    url = argv[optind];

    CURL* ch = curl_easy_init();
    curl_easy_setopt(ch, CURLOPT_URL, url);
    curl_easy_setopt(ch, CURLOPT_VERBOSE, verbose);
    if (strcmp((const char*)method, "POST") == 0) {
        curl_easy_setopt(ch, CURLOPT_POST, 1);
    }
    //curl_easy_setopt(ch, CURLOPT_HEADER, head);
    //curl_easy_setopt(ch, CURLOPT_NOBODY, head);
    //curl_easy_setopt(ch, CURLOPT_HEADERFUNCTION, read_header);

    CURLcode code;
    do {
        code = curl_easy_perform(ch);
        count--;
        usleep(interval * 1000);
    } while(count == -1 || count);

    curl_easy_cleanup(ch);

    return 0;
}
