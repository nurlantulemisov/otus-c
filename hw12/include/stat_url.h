#ifndef STAT_URL
#define STAT_URL

#include <sys/_pthread/_pthread_mutex_t.h>
#include <sys/_types/_size_t.h>

typedef struct {
  pthread_mutex_t mu;
  size_t bytes;
} atomic_;

#endif // !STAT_URL
