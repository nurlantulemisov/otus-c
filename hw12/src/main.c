#include "circular_buffer.h"
#include "hashtable.h"
#include "thread_pool.h"

#include <dirent.h>
#include <errno.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/_types/_size_t.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define MAX_LEN 9216

atomic_size_t bytes = 0;

hash_t *refer_hash_t = NULL;
hash_t *uri_hash_t = NULL;

uint64_t
parse_apache_log(char *log) {
  char *start, *end;

  // IP
  end = (char *) memchr(log, ' ', strlen(log));
  char ip[end - log + 1];
  strncpy(ip, log, end - log);
  ip[end - log] = '\0';

  // Request
  start = (char *) memchr(log, '"', strlen(log)) + 1;
  end = (char *) memchr(start, '"', strlen(start));
  char request[end - start + 1];
  strncpy(request, start, end - start);
  request[end - start] = '\0';

  char *uri;
  uri = strtok(request, " ");
  uri = strtok(NULL, " ");

  start = (char *) memchr(end + 1, ' ', strlen(end + 1)) + 1;
  end = (char *) memchr(start, ' ', strlen(start));
  char status[end - start + 1];
  strncpy(status, start, end - start);
  status[end - start] = '\0';

  // Bytes
  start = (char *) memchr(end, ' ', strlen(end + 1)) + 1;
  end = (char *) memchr(start, '"', strlen(start));
  char bytes[end - start + 1];
  strncpy(bytes, start, end - start);
  bytes[end - start] = '\0';
  uint64_t number = strtoull(bytes, NULL, 10);

  // Referer
  start = (char *) memchr(end, '"', strlen(end + 1)) + 1;
  end = (char *) memchr(start, '"', strlen(start));
  char referer[end - start + 1];
  strncpy(referer, start, end - start);
  referer[end - start] = '\0';

  const char *ref = strtok(referer, " ");

  if(ref != NULL && ref[0] != '-') {
    hash_t_inc(refer_hash_t, ref, 1);
  }

  if(uri != NULL) {
    hash_t_inc(uri_hash_t, uri, number);
  }

  return number;
}

void
process_file(void *arg) {
  const char *filepath = (char *) arg;
  if(filepath == NULL) {
    return;
  }

  printf("processing file: %s\n", filepath);

  FILE *file = fopen(filepath, "r"); // Открываем файл для чтения
  if(file == NULL) {
    perror("failed to open file");
    return;
  }

  char line[MAX_LEN];
  uint64_t current_bytes = 0;
  while(fgets(line, MAX_LEN, file) != NULL) {
    current_bytes += parse_apache_log(line);
  }

  printf("finish file: %s\n", filepath);
  atomic_fetch_add(&bytes, current_bytes);

  fclose(file);

  return;
}

int
main(int argc, char *argv[]) {
  if(argc != 3) {
    printf("argc=%d. logparser <num_thread> <log_dir>", argc);
    return EXIT_FAILURE;
  }

  const char *n_thread = argv[1];
  if(n_thread == NULL) {
    perror("num_thread is not passed. logparser <num_thread> <log_dir>");
    return EXIT_FAILURE;
  }

  const char *dirname = argv[2];
  if(dirname == NULL) {
    perror("dirname is not passed. logparser <num_thread> <log_dir>");
    return EXIT_FAILURE;
  }

  DIR *dir = opendir(dirname);
  if(dir == NULL) {
    perror("error opening directory");
    return EXIT_FAILURE;
  }

  struct dirent *entry;
  size_t file_count = 0;
  while((entry = readdir(dir)) != NULL) {
    if(entry->d_type == DT_REG) {
      file_count++;
    }
  }

  closedir(dir);

  size_t num_thread = atoi(n_thread);
  if(num_thread <= 0 || num_thread > file_count) {
    num_thread = file_count;
    printf("num_thread set default value. ");
  }
  printf("num_thread=%zu\n", num_thread);

  cir_buffer_t *buf = create_cir_buffer(10);
  if(buf == NULL) {
    perror("failed create cir buffer");
    return EXIT_FAILURE;
  }

  refer_hash_t = create_hash_t(100);
  if(refer_hash_t == NULL) {
    perror("failed init hash table");
    return EXIT_FAILURE;
  }

  uri_hash_t = create_hash_t(100);
  if(uri_hash_t == NULL) {
    perror("failed init hash table");
    return EXIT_FAILURE;
  }

  thread_pool_t *pool = create_pool(num_thread, buf);
  if(pool == NULL) {
    perror("failed create thread pool");
    return EXIT_FAILURE;
  }

  dir = opendir(dirname);
  if(dir == NULL) {
    perror("Error opening directory");
    return EXIT_FAILURE;
  }

  while((entry = readdir(dir)) != NULL) {
    if(entry->d_type == DT_REG) {
      char full_path[120];
      snprintf(full_path, sizeof(full_path), "%s/%s", dirname, entry->d_name);
      add_to_pool(pool, full_path, process_file);
    }
  }

  closedir(dir);

  while(cir_buffer_size(buf) != 0) {
    sleep(1);
  }

  stop_pool(pool);

  printf("\nTop 10 refer: \n");
  print_top(refer_hash_t, 10);
  printf("\nTop 10 the haviest uri: \n");
  print_top(uri_hash_t, 10);

  hash_t_free(refer_hash_t);
  hash_t_free(uri_hash_t);
  printf("\nMax size: %zu Gb\n", atomic_load(&bytes) / 1024 / 1024 / 1024);
  return EXIT_SUCCESS;
}
