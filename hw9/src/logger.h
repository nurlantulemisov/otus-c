#ifndef LOGGER_H
#define LOGGER_H

#include <stdbool.h>
#include <string.h>

#define __FILENAME__                                                           \
  (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define LOG_TRACE(fmt, ...)                                                    \
  logger_log(LogLevel_TRACE, __FILENAME__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...)                                                    \
  logger_log(LogLevel_DEBUG, __FILENAME__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)                                                     \
  logger_log(LogLevel_INFO, __FILENAME__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)                                                     \
  logger_log(LogLevel_WARN, __FILENAME__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...)                                                    \
  logger_log(LogLevel_ERROR, __FILENAME__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_FATAL(fmt, ...)                                                    \
  logger_log(LogLevel_FATAL, __FILENAME__, __LINE__, fmt, ##__VA_ARGS__)

typedef enum {
  LogLevel_TRACE,
  LogLevel_DEBUG,
  LogLevel_INFO,
  LogLevel_WARN,
  LogLevel_ERROR,
  LogLevel_FATAL,
} LogLevel;

/**
 * Log a message.
 * Make sure to call one of the following initialize functions before starting
 * logging.
 * - logger_initConsoleLogger()
 * - logger_initFileLogger()
 *
 * @param[in] level A log level
 * @param[in] file A file name string
 * @param[in] line A line number
 * @param[in] fmt A format string
 * @param[in] ... Additional arguments
 */
void logger_log(LogLevel level, const char *file, int line, const char *fmt,
                ...);

bool init_logger(const char *f);

#endif
