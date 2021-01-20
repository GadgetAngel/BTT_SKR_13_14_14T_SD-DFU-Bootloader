#ifndef _MIN_PRINTF_H
#define _MIN_PRINTF_H

#include <stdarg.h>

// #define printf(a) _puts(a)
#define puts(...) _puts(0, __VA_ARGS__)
int _puts(int fd, const char *str);
int vfprintf(int fd, const char *format, va_list args);
int fprintf(int fd, const char *format, ...) __attribute__ ((__format__ (__printf__, 2, 3)));
int printf(const char *format, ...) __attribute__ ((__format__ (__printf__, 1, 2)));

#endif /* _MIN_PRINTF_H */
