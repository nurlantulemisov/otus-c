#include "logger.h"

#include <execinfo.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

static bool is_init = false;
pthread_mutex_t mutex;

/* File logger */
static struct {
  FILE *output;
  char filename[256];
  uint32_t maxFileSize;
  off_t currentFileSize;
  uint64_t flushedTime;
} s_flog;

static off_t
log_file_size(const char *filename) {
  struct stat st;
  stat(filename, &st);
  return st.st_size;
}

bool
init_logger(const char *f) {
  if(is_init) {
    return true;
  }

  pthread_mutex_init(&mutex, NULL);
  pthread_mutex_lock(&mutex);

  FILE *log_file = fopen(f, "a");
  if(log_file == NULL) {
    perror("failed open file");
    return false;
  }
  strncpy(s_flog.filename, f, sizeof(s_flog.filename));
  s_flog.output = log_file;
  s_flog.maxFileSize = 1024 * 1024 * 10; // 10mb
  s_flog.currentFileSize = log_file_size(f);

  is_init = true;

  pthread_mutex_unlock(&mutex);

  return true;
}

static bool
log_rotate() {
  if(s_flog.currentFileSize < s_flog.maxFileSize) {
    return s_flog.output != NULL;
  }

  fclose(s_flog.output);

  struct timespec tms;

  if(clock_gettime(CLOCK_REALTIME, &tms)) {
    return s_flog.output != NULL;
  }

  char new[2 * 256];
  char timestamp[100];
  snprintf(timestamp, sizeof(timestamp), "%lld.%.9ld", (long long) tms.tv_sec,
	   tms.tv_nsec);

  snprintf(new, sizeof(s_flog.filename) + sizeof(timestamp), "%s.%s",
	   s_flog.filename, timestamp);

  if(rename(s_flog.filename, new)) {
    return false;
  }

  s_flog.output = fopen(s_flog.filename, "a");
  if(s_flog.output == NULL) {
    return false;
  }

  return true;
}

static char
getLevelChar(LogLevel level) {
  switch(level) {
  case LogLevel_TRACE:
    return 'T';
  case LogLevel_DEBUG:
    return 'D';
  case LogLevel_INFO:
    return 'I';
  case LogLevel_WARN:
    return 'W';
  case LogLevel_ERROR:
    return 'E';
  case LogLevel_FATAL:
    return 'F';
  default:
    return ' ';
  }
}

static bool
print_backtrace(FILE *file) {
  void *buffer[10];
  int nptrs = backtrace(buffer, 10);
  char **strings = backtrace_symbols(buffer, nptrs);
  if(strings == NULL) {
    perror("backtrace_symbols");
    return false;
  }
  fprintf(file, "Backtrace:\n");
  for(int i = 0; i < nptrs; i++) {
    fprintf(file, "%s\n", strings[i]);
  }
  free(strings);
  return true;
}

void
logger_log(LogLevel level, const char *file, int line, const char *fmt, ...) {
  if(is_init) {
    return;
  }

  uint64_t total_size = 0;
  va_list farg;

  pthread_mutex_lock(&mutex);

  if(log_rotate()) {
    size_t size = 0;
    if((size =
	  fprintf(s_flog.output, "%c %s:%d: ", getLevelChar(level), file, line))
       > 0) {
      total_size += size;
    }
    va_start(farg, fmt);
    if((size = vfprintf(s_flog.output, fmt, farg)) > 0) {
      total_size += size;
    }
    va_end(farg);

    if((size = fprintf(s_flog.output, "\n")) > 0) {
      total_size += size;
    }

    if(level == LogLevel_ERROR) {
      print_backtrace(s_flog.output);
    }

    s_flog.currentFileSize += total_size;
  }

  pthread_mutex_unlock(&mutex);
}
