#include "curl.h"

#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t
write_response(void *contents, size_t size, size_t nmemb, void *userdata) {
  HTTPCli *http_cli = (HTTPCli *) userdata;
  size_t realsize = size * nmemb;
  http_cli->response =
    realloc(http_cli->response, http_cli->response
				  ? strlen(http_cli->response) + realsize + 1
				  : realsize + 1);
  if(http_cli->response == NULL) {
    return 0;
  }
  memcpy(&(http_cli->response[strlen(http_cli->response)]), contents, realsize);
  http_cli->response[strlen(http_cli->response) + realsize] = 0;
  return realsize;
}

HTTPCli *
new_http_cli(void) {
  HTTPCli *http_cli = (HTTPCli *) malloc(sizeof(HTTPCli));
  if(http_cli == NULL) {
    return NULL;
  }
  http_cli->response = malloc(1);
  http_cli->response = NULL;

  curl_global_init(CURL_GLOBAL_ALL);

  http_cli->curl = (CURL *) malloc(sizeof(http_cli->curl));
  if(http_cli->curl == NULL) {
    return NULL;
  }

  http_cli->curl = curl_easy_init();

  curl_easy_setopt(http_cli->curl, CURLOPT_WRITEFUNCTION, write_response);
  curl_easy_setopt(http_cli->curl, CURLOPT_WRITEDATA, (void *) http_cli);
  curl_easy_setopt(http_cli->curl, CURLOPT_TIMEOUT, 5L);

  return http_cli;
}

char *
do_request(HTTPCli *cli, char *url) {
  curl_easy_setopt(cli->curl, CURLOPT_URL, url);
  CURLcode code = curl_easy_perform(cli->curl);
  if(code != CURLE_OK) {
    fprintf(stderr, "curl failed: %s\n", curl_easy_strerror(code));
    return NULL;
  }

  return cli->response;
}

void
clear(HTTPCli *cli) {
  curl_global_cleanup();
  curl_easy_cleanup(cli->curl);
  free(cli->response);
  free(cli);
}
