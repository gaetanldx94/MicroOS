#ifndef KERNEL_H
#define KERNEL_H

#include <cstdint>

extern "C" void kernelMain(void *multiboot_structure, uint32_t magicnumber);

#endif
