#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>

typedef struct
{
    CURL *curl;
    char *response;
    uint32_t timeout;
} HTTPCli;

size_t write_response(void *contents, size_t size, size_t nmemb, void *userdata)
{
    HTTPCli *http_cli = (HTTPCli *)userdata;
    size_t realsize = size * nmemb;
    http_cli->response = realloc(http_cli->response, http_cli->response ? strlen(http_cli->response) + realsize + 1 : realsize + 1);
    if (http_cli->response == NULL)
    {
        return 0;
    }
    memcpy(&(http_cli->response[strlen(http_cli->response)]), contents, realsize);
    http_cli->response[strlen(http_cli->response) + realsize] = 0;
    return realsize;
}

HTTPCli *new_http_cli()
{
    HTTPCli *http_cli = (HTTPCli *)malloc(sizeof(HTTPCli));
    if (http_cli == NULL)
    {
        return NULL;
    }
    http_cli->response = malloc(1);
    http_cli->response = '\0';

    curl_global_init(CURL_GLOBAL_ALL);

    http_cli->curl = (CURL *)malloc(sizeof(CURL));
    if (http_cli->curl == NULL)
    {
        return NULL;
    }

    http_cli->curl = curl_easy_init();

    curl_easy_setopt(http_cli->curl, CURLOPT_WRITEFUNCTION, write_response);
    curl_easy_setopt(http_cli->curl, CURLOPT_WRITEDATA, (void *)http_cli);
    curl_easy_setopt(http_cli->curl, CURLOPT_TIMEOUT, 5L);
}
