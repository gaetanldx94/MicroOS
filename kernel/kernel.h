#ifndef KERNEL_H
#define KERNEL_H

#include <cstdint>
#include <cstdarg>

extern "C" void kernel_printf(const char *format, ...);
extern "C" void kernel_printf_int(int value);

#endif // KERNEL_H
