#include "logger.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  printf("hello world");

  init_logger("logfile.txt");

  LOG_INFO("%s %d", "infoooo", 10);
  LOG_DEBUG("%s %d", "infoooo", 10);

  LOG_FATAL("%lld", (long long)10);

  LOG_ERROR("%s %d", "infoooo", 10);

  void *ptr = malloc(0);
  if (ptr == NULL) {
    LOG_ERROR("malloc err");
  }

  return EXIT_SUCCESS;
}
