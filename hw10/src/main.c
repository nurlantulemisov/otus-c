#include <confuse.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/syslog.h>
#include <sys/un.h>
#include <syslog.h>
#include <unistd.h>

#define LOCKFILE "/Users/n.tulemisov/myprojects/otus-c/build/usd.pid"
#define LOCKMODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define SOCK_NAME "usd.socket"

static cfg_bool_t verbose = cfg_false;
static char *look_f = NULL;
volatile sig_atomic_t flag = 0;

void
init_conf(const char *conf_path) {
  cfg_opt_t opts[] = {CFG_SIMPLE_BOOL("verbose", &verbose),
		      CFG_SIMPLE_STR("path", &look_f), CFG_END()};

  cfg_t *cfg = cfg_init(opts, 0);
  cfg_parse(cfg, conf_path);
  cfg_free(cfg);
}

void
sigterm_handler(int signum) {
  flag = 1;
}

int
lockfile(int fd) {
  struct flock fl;
  fl.l_type = F_WRLCK;
  fl.l_start = 0;
  fl.l_whence = SEEK_SET;
  fl.l_len = 0;

  return (fcntl(fd, F_SETLK, &fl));
}

bool
validate_is_already_worked(void) {
  char buf[16];
  int fd = open(LOCKFILE, O_RDWR | O_CREAT, LOCKMODE);
  if(fd < 0) {
    syslog(LOG_ERR, "failed open %s: %s", LOCKFILE, strerror(errno));
    return true;
  }
  if(lockfile(fd) < 0) {
    if(errno == EACCES || errno == EAGAIN) {
      close(fd);
      return true;
    }
    syslog(LOG_ERR, "failed to set lock %s: %s", LOCKFILE, strerror(errno));
    return true;
  }
  ftruncate(fd, 0);
  sprintf(buf, "%ld", (long) getpid());
  write(fd, buf, strlen(buf) + 1);

  return false;
}

void
print_debug(const char *format, ...) {
  if(verbose) {
    va_list args;
    va_start(args, format);
    syslog(LOG_DEBUG, format, args);
    va_end(args);
  }
}

bool
unix_socket_server(void) {
  int sock = socket(AF_UNIX, SOCK_STREAM, 0);
  if(sock < 0) {
    perror("opening stream socket");
    return false;
  }

  struct sockaddr_un server;
  server.sun_family = AF_UNIX;
  strcpy(server.sun_path, SOCK_NAME);
  if(bind(sock, (struct sockaddr *) &server, sizeof(struct sockaddr_un))) {
    perror("binding stream socket");
    return false;
  }

  syslog(LOG_INFO, "listen socket: %s", server.sun_path);
  listen(sock, 5);

  struct stat st;
  char buf[100];
  while(!flag) {
    int msgsock = accept(sock, 0, 0);
    if(msgsock == -1) {
      syslog(LOG_ERR, "accept. %s", strerror(errno));
      break;
    }

    print_debug("new connection");

    if(stat(look_f, &st) == 0) {
      snprintf(buf, sizeof(buf), "File size %s: %lld byte\n", look_f,
	       st.st_size);
    } else {
      snprintf(buf, sizeof(buf), "Not access to file %s\n", look_f);
    }

    send(msgsock, buf, strlen(buf), 0);
    close(msgsock);
  }

  syslog(LOG_INFO, "close unix socket");
  close(sock);
  unlink(SOCK_NAME);

  return true;
}

void
daemonize(void) {
  openlog("usd", LOG_PID | LOG_NDELAY, LOG_DAEMON);
  /*
   * Сбросить маску режима создания файла.
   */
  umask(0);
  /*
   * Получить максимально возможный номер дескриптора файла.
   */
  struct rlimit rl;
  if(getrlimit(RLIMIT_NOFILE, &rl) < 0)
    perror("невозможно получить максимальный номер дескриптора");
  /*
   * Стать лидером нового сеанса, чтобы утратить управляющий терминал.
   */
  pid_t pid;
  if((pid = fork()) < 0)
    perror("ошибка вызова функции fork");
  else if(pid != 0) /* родительский процесс */
    exit(0);
  setsid();
  /*
   * Обеспечить невозможность обретения управляющего терминала в будущем.
   */
  struct sigaction sa;
  sa.sa_handler = SIG_IGN;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  if(sigaction(SIGHUP, &sa, NULL) < 0)
    syslog(LOG_CRIT, "невозможно игнорировать сигнал SIGHUP");
  if((pid = fork()) < 0)
    syslog(LOG_CRIT, "ошибка вызова функции fork");
  else if(pid != 0) /* родительский процесс */
    exit(0);
  /*
   * Назначить корневой каталог текущим рабочим каталогом,
   * чтобы впоследствии можно было отмонтировать файловую систему.
   */
  if(chdir("/Users/n.tulemisov/myprojects/otus-c/build/") < 0)
    syslog(LOG_CRIT, "невозможно сделать текущим рабочим каталогом /");
  /*
   * Закрыть все открытые файловые дескрипторы.
   */
  if(rl.rlim_max == RLIM_INFINITY)
    rl.rlim_max = 1024;
  for(int i = 0; i < rl.rlim_max; i++)
    close(i);
  /*
   * Присоединить файловые дескрипторы 0, 1 и 2 к /dev/null.
   */
  int fd0 = open("/dev/null", O_RDWR);
  int fd1 = dup(0);
  int fd2 = dup(0);
  if(fd0 != 0 || fd1 != 1 || fd2 != 2)
    syslog(LOG_CRIT, "ошибочные файловые дескрипторы %d %d %d", fd0, fd1, fd2);
}

int
main(int argc, char *argv[]) {
  char *path_conf = argv[1];

  if(path_conf == NULL) {
    perror("conf file is not exist");
    return EXIT_FAILURE;
  }

  const char *d = argv[1];

  if(strncmp(d, "-d", sizeof("-d")) == 0
     || strncmp(d, "--daemon", sizeof("--daemon")) == 0) {
    path_conf = argv[2];
    if(path_conf == NULL) {
      syslog(LOG_ERR, "%s", strerror(errno));
      perror("conf file is not exist");
      return EXIT_FAILURE;
    }

    daemonize();
  }
  openlog("usd", LOG_PID | LOG_NDELAY, LOG_LPR);

  if(validate_is_already_worked()) {
    perror("already open");
    syslog(LOG_ERR, "already open %s", strerror(errno));
    return EXIT_FAILURE;
  }

  syslog(LOG_INFO, "open conf file: %s", path_conf);

  init_conf(path_conf);

  if(look_f == NULL || access(look_f, F_OK) == -1) {
    perror("watching file is not exist");
    syslog(LOG_ERR, "watching file not exist %s", strerror(errno));
    return EXIT_FAILURE;
  }

  syslog(LOG_INFO, "file %s for watching", look_f);

  struct sigaction sa;
  sa.sa_handler = sigterm_handler;
  sa.sa_flags = 0;
  sigemptyset(&sa.sa_mask);

  if(sigaction(SIGINT, &sa, NULL) == -1) {
    perror("sig custom handler error");
    return EXIT_FAILURE;
  }

  print_debug("init custom sigint_handler");

  if(!unix_socket_server()) {
    perror("failed run unix socket");
    syslog(LOG_ERR, "run socket %s", strerror(errno));
    return EXIT_FAILURE;
  }

  closelog();
  return EXIT_SUCCESS;
}
