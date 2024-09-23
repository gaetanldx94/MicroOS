#include "includes/interrupts.h"
#include "includes/video.h"
#include <cstdint>

struct idt_entry
{
	uint16_t base_lo;
	uint16_t sel;
	uint8_t always0;
	uint8_t flags;
	uint16_t base_hi;
} __attribute__((packed));

struct idt_ptr
{
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));

#define IDT_ENTRIES 256
idt_entry idt[IDT_ENTRIES];
idt_ptr idtp;

void idt_set_gate(int num, uint32_t base, uint16_t sel, uint8_t flags)
{
	idt[num].base_lo = base & 0xFFFF;
	idt[num].base_hi = (base >> 16) & 0xFFFF;
	idt[num].sel = sel;
	idt[num].always0 = 0;
	idt[num].flags = flags;
}

extern "C" void idt_load(uint32_t);

void idt_install()
{
	idtp.limit = (sizeof(idt_entry) * IDT_ENTRIES) - 1;
	idtp.base = (uint32_t)&idt;

	for (int i = 0; i < IDT_ENTRIES; i++)
	{
		idt_set_gate(i, 0, 0, 0);
	}

	idt_load((uint32_t)&idtp);
	test_message("Chargement de l'IDT", true);
}

extern "C" void isr_handler(uint32_t int_no)
{
	kernel_printf("Interruption reçue: ");
	char num[3] = {(char)('0' + int_no / 10), (char)('0' + int_no % 10), '\0'};
	kernel_printf(num);
	kernel_printf("\n");
}