#include "circular_buffer.h"
#include "thread_pool.h"
#include <dirent.h>
#include <errno.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_LEN 256

atomic_uint bytes = ATOMIC_VAR_INIT(0);

uint64_t parse_apache_log(const char *s) {
  char log_line[MAX_LEN];
  char ip[MAX_LEN], user[MAX_LEN], datetime[MAX_LEN], request[MAX_LEN],
      status[MAX_LEN], size[MAX_LEN], referer[MAX_LEN], user_agent[MAX_LEN];

  // Пример строки журнала доступа
  strcpy(log_line, s);

  // Используем sscanf для распарсинга строки
  sscanf(log_line, "%s %s %s [%[^]]] \"%[^\"]\" %s %s \"%[^\"]\" \"%[^\"]\"",
         ip, user, datetime, request, status, size, referer, user_agent);

  // Выводим распарсенные данные
  // printf("IP: %s\n", ip);
  // printf("User: %s\n", user);
  // printf("Datetime: %s\n", datetime);
  // printf("Request: %s\n", request);
  // printf("Status: %s\n", status);
  // printf("Size: %s\n", size);
  // printf("Referer: %s\n", referer);
  // printf("User Agent: %s\n", user_agent);

  // Преобразование строки size в size_t

  return strtoull(size, NULL, 10);
}

void process_file(void *arg) {
  const char *filepath = (char *)arg;
  if (filepath == NULL) {
    return;
  }

  // Здесь выполняется обработка файла
  printf("Processing file: %s\n", filepath);

  FILE *file = fopen(filepath, "r"); // Открываем файл для чтения
  if (file == NULL) {
    perror("Failed to open file");
    return;
  }
  char line[MAX_LEN];
  uint64_t current_bytes = 0;
  while (fgets(line, sizeof(line), file) != NULL) {
    // Обработка прочитанной строки
    current_bytes += parse_apache_log(line);
  }
  printf("%llu\n", current_bytes);
  atomic_fetch_add(&bytes, current_bytes);

  return;
}

int main(int argc, char *argv[]) {
  // atomic_size_t bytes = ATOMIC_VAR_INIT(0);
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

  thread_pool_t *pool = create_pool(8, buf);
  if (pool == NULL) {
    perror("failed create thread pool");
    return EXIT_FAILURE;
  }

  const char *dirname = argv[1];
  if (dirname == NULL) {
    perror("dirname not passed");
    return EXIT_FAILURE;
  }

  DIR *dir = opendir(dirname);
  if (dir == NULL) {
    perror("Error opening directory");
    return EXIT_FAILURE;
  }
  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    if (entry->d_type == DT_REG) {
      // +2 для разделителя и завершающего нулевого символа
      size_t p_size = strlen(dirname) + strlen(entry->d_name) + 2;
      char *full_path = (char *)malloc(p_size);

      if (full_path != NULL) {
        snprintf(full_path, p_size, "%s/%s", dirname, entry->d_name);
        add_to_pool(pool, full_path, process_file);
      }
    }
  }

  closedir(dir);

  while (cir_buffer_size(buf) != 0) {
    sleep(1);
  }

  stop_pool(pool);
  printf("Counter: %lld\n", atomic_load(&bytes));
  return EXIT_SUCCESS;
}
