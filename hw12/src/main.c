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
#include <sys/stat.h>
#include <unistd.h>

#define MAX_LEN 1000

atomic_size_t bytes = ATOMIC_VAR_INIT(0);

hash_t *t = NULL;

struct stat_url {
  const char *request;
  const char *refer;
  uint64_t bytes;
};

uint64_t parse_apache_log(char *s) {
  char ip[16], date[30], request[1000], referrer[1000], userAgent[1000];
  int status, size;

  sscanf(s, "%s - - %[^ ] \"%[^\"]\" %d %d \"%[^\"]\" \"%[^\"]\"", ip, date,
         request, &status, &size, referrer, userAgent);

  printf("IP: %s\n", ip);
  printf("Date: %s\n", date);
  printf("Request: %s\n", request);
  printf("Status: %d\n", status);
  printf("Size: %d\n", size);
  printf("Referrer: %s\n", referrer);
  printf("User Agent: %s\n", userAgent);
}

// uint64_t parse_apache_log(char *s) {
//   char ip[16];
//   char date_time[27];
//   char request[256];
//   char method[8];
//   char uri[256];
//   char protocol[16];
//   int status;
//   int size;
//   char referer[256];
//   char user_agent[256];
//
//   sscanf(s, "%s - - [%[^]]] \"%[^\"]\" %u %d \"%[^\"]\" \"%[^\"]\"", ip,
//          date_time, request, &status, &size, referer, user_agent);
//
//   // sscanf(request, "%s %s %s", method, uri, protocol);
//
//   // printf("IP: %s\n", ip);
//   // printf("Date and Time: %s\n", date_time);
//   // printf("Method: %s\n", method);
//   // printf("URI: %s\n", uri);
//   // printf("Protocol: %s\n", protocol);
//   // printf("Status: %d\n", status);
//   // printf("Size: %llu\n", size);
//   // printf("Referer: %s\n", referer);
//   // printf("User Agent: %s\n", user_agent);
//
//   return 0;
//   // return (struct stat_url){.request = uri, .refer = referer, .bytes =
//   size};
// }

void process_file(void *arg) {
  const char *filepath = (char *)arg;
  if (filepath == NULL) {
    return;
  }

  printf("Processing file: %s\n", filepath);

  FILE *file = fopen(filepath, "r"); // Открываем файл для чтения
  if (file == NULL) {
    perror("Failed to open file");
    return;
  }

  // hash_t *ht = create_hash_t(1000);

  char line[MAX_LEN];
  uint64_t current_bytes = 0;
  while (fgets(line, sizeof(line), file) != NULL) {
    // printf("line: %s\n", line);
    current_bytes += parse_apache_log(line);
    // current_bytes += s.bytes;
    // hash_t_inc(ht, s.request);
  }
  printf("finish file: %s\n", filepath);
  atomic_fetch_add(&bytes, current_bytes);

  // print_hash_t(ht);
  // hash_t_free(ht);

  return;
}

int main(int argc, char *argv[]) {
  parse_apache_log(
      "62.148.157.93 - - [26/May/2020:12:10:31 +0000] \"GET "
      "/%D0%9C%D0%B0%D1%80%D0%B8%D0%B2%D0%B0%D0%BD%D0%BD%D0%B0-%D0%B4%D0%B5%D1%"
      "82%D0%B8-%D0%BF%D1%80%D0%B8%D0%B4%D1%83%D0%BC%D0%B0%D0%B9%D1%82%D0%B5-%"
      "D0%BF%D1%80%D0%B5%D0%B4%D0%BB%D0%BE%D0%B6%D0%B5%D0%BD%D0%B8%D1%8F-%D1%"
      "81%D0%BE-%D1%81%D0%BB%D0%BE%D0%B2%D0%B0%D0%BC%D0%B8/ HTTP/2.0\" 200 "
      "10746 "
      "\"https://baneks.site/"
      "%D0%9C%D0%B0%D1%80%D0%B8%D0%B2%D0%B0%D0%BD%D0%BD%D0%B0-%D0%B4%D0%B5%D1%"
      "82%D0%B8-%D0%BF%D1%80%D0%B8%D0%B4%D1%83%D0%BC%D0%B0%D0%B9%D1%82%D0%B5-%"
      "D0%BF%D1%80%D0%B5%D0%B4%D0%BB%D0%^CBE%D0%B6%D0%B5%D0%BD%D0%B8%D1%8F-%D1%"
      "81%D0%BE-%D1%81%D0%BB%D0%BE%D0%B2%D0%B0%D0%BC%D0%B8/"
      "Mozilla/5.0 (Linux; U; Android 8.1.0; ru-ru; Redmi 5A "
      "Build/OPM1.171019.026) AppleWebKit/537.36 (KHTML, like Gecko) "
      "Version/4.0 Chrome/71.0.3578.141 Mobile Safari/537.36 "
      "XiaoMi/MiuiBrowser/12.2.3-g\"");

  // cir_buffer_t *buf = create_cir_buffer(10);
  // if (buf == NULL) {
  //   perror("failed create cir buffer");
  //   return EXIT_FAILURE;
  // }
  //
  // // t = create_hash_t(100);
  //
  // thread_pool_t *pool = create_pool(1, buf);
  // if (pool == NULL) {
  //   perror("failed create thread pool");
  //   return EXIT_FAILURE;
  // }
  //
  // const char *dirname = argv[1];
  // if (dirname == NULL) {
  //   perror("dirname not passed");
  //   return EXIT_FAILURE;
  // }
  //
  // DIR *dir = opendir(dirname);
  // if (dir == NULL) {
  //   perror("Error opening directory");
  //   return EXIT_FAILURE;
  // }
  // struct dirent *entry;
  // while ((entry = readdir(dir)) != NULL) {
  //   if (entry->d_type == DT_REG) {
  //     // +2 для разделителя и завершающего нулевого символа
  //     char full_path[MAX_LEN];
  //     snprintf(full_path, sizeof(full_path), "%s/%s", dirname,
  //     entry->d_name); add_to_pool(pool, full_path, process_file);
  //   }
  // }
  //
  // closedir(dir);
  //
  // while (cir_buffer_size(buf) != 0) {
  //   sleep(1);
  // }
  //
  // stop_pool(pool);
  //
  // // print_hash_t(t);
  //
  // // hash_t_free(t);
  // printf("Counter: %zu\n", atomic_load(&bytes));
  return EXIT_SUCCESS;
}
