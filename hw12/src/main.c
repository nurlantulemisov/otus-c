#include "circular_buffer.h"
#include "thread_pool.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LEN 256

void parse_apache_log(const char *s) {
  char log_line[MAX_LEN];
  char ip[MAX_LEN], user[MAX_LEN], datetime[MAX_LEN], request[MAX_LEN],
      status[MAX_LEN], size[MAX_LEN], referer[MAX_LEN], user_agent[MAX_LEN];

  // Пример строки журнала доступа
  strcpy(log_line, s);

  // Используем sscanf для распарсинга строки
  sscanf(log_line, "%s %s %s [%[^]]] \"%[^\"]\" %s %s \"%[^\"]\" \"%[^\"]\"",
         ip, user, datetime, request, status, size, referer, user_agent);

  // Выводим распарсенные данные
  printf("IP: %s\n", ip);
  printf("User: %s\n", user);
  printf("Datetime: %s\n", datetime);
  printf("Request: %s\n", request);
  printf("Status: %s\n", status);
  printf("Size: %s\n", size);
  printf("Referer: %s\n", referer);
  printf("User Agent: %s\n", user_agent);
}

int main(int argc, char *argv[]) {
  // Пример строки журнала доступа
  parse_apache_log(
      "127.0.0.1 - frank [10/Oct/2000:13:55:36 -0700] \"GET /apache_pb.gif "
      "HTTP/1.0\" 200 2326 \"http://www.example.com/start.html\" "
      "\"Mozilla/4.08 [en] (Win98; I ;Nav)\"");

  cir_buffer_t *buf = create_cir_buffer(10);
  if (buf == NULL) {
    perror("failed create cir buffer");
    return EXIT_FAILURE;
  }

  thread_pool_t *pool = create_pool(2, buf);
  if (pool == NULL) {
    perror("failed create thread pool");
    return EXIT_FAILURE;
  }
  char s[5];
  for (int i = 0; i < 100; ++i) {
    snprintf(s, sizeof(s), "%d\n", i);
    char *str = strdup(s);
    if (str == NULL) {
      perror("failed to duplicate string");
      return EXIT_FAILURE;
    }
    add_to_pool(pool, str);
  }

  while (cir_buffer_size(buf) != 0) {
    sleep(1);
  }

  stop_pool(pool);

  return EXIT_SUCCESS;
}
