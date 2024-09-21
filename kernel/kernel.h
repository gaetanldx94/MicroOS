#ifndef KERNEL_H
#define KERNEL_H

#include <cstdint>

extern "C" void kernel_printf(const char *str);
extern "C" void kernel_printf_int(int value);

#endif // KERNEL_H
