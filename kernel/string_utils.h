#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <cstddef>
#include <cstdarg>

size_t my_strlen(const char *str);
void my_strncpy(char *dest, const char *src, size_t n);
int my_strcmp(const char *str1, const char *str2);
void my_snprintf(char *buffer, size_t size, const char *format, va_list args);
int my_strncmp(const char *str1, const char *str2, size_t n);

#endif