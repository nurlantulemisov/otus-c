#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  CURL *curl;
  char *response;
  uint32_t timeout;
} HTTPCli;

HTTPCli *
new_http_cli(void);
char *
do_request(HTTPCli *cli, char *url);
void
clear(HTTPCli *cli);
