#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <cstdint>

void idt_install();
void idt_set_gate(int num, uint32_t base, uint16_t sel, uint8_t flags);

#endif
