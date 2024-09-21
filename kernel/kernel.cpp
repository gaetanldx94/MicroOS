#include <cstdint>
#include <cstdio>
#include "memory.h"
#include "process.h"

// Déclaration de itoa
void itoa(int num, char *buffer);

const int VGA_WIDTH = 80;
const int VGA_HEIGHT = 25;
unsigned short *VideoMemory = (unsigned short *)0xb8000;
int cursor_x = 0;
int cursor_y = 0;

// Déclarations externes
extern "C" void init_process_manager();
extern "C" void context_switch();
extern "C" void process_entry(uint32_t pid, const char *process_name); // Assurez-vous que cette fonction est définie

extern "C" void outb(uint16_t port, uint8_t value)
{
	asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

void move_cursor()
{
	unsigned short cursorLocation = cursor_y * VGA_WIDTH + cursor_x;
	outb(0x3D4, 14);
	outb(0x3D5, cursorLocation >> 8);
	outb(0x3D4, 15);
	outb(0x3D5, cursorLocation);
}

void clear_screen()
{
	for (int y = 0; y < VGA_HEIGHT; y++)
	{
		for (int x = 0; x < VGA_WIDTH; x++)
		{
			VideoMemory[y * VGA_WIDTH + x] = (VideoMemory[y * VGA_WIDTH + x] & 0xFF00) | ' ';
		}
	}
	cursor_x = 0;
	cursor_y = 0;
	move_cursor();
}

extern "C" void kernel_printf(const char *str)
{
	for (int i = 0; str[i] != '\0'; ++i)
	{
		char c = str[i];
		if (c == '\n')
		{
			cursor_x = 0;
			cursor_y++;
		}
		else
		{
			VideoMemory[cursor_y * VGA_WIDTH + cursor_x] = (VideoMemory[cursor_y * VGA_WIDTH + cursor_x] & 0xFF00) | c;
			cursor_x++;
			if (cursor_x >= VGA_WIDTH)
			{
				cursor_x = 0;
				cursor_y++;
			}
		}

		if (cursor_y >= VGA_HEIGHT)
		{
			for (int y = 0; y < VGA_HEIGHT - 1; y++)
			{
				for (int x = 0; x < VGA_WIDTH; x++)
				{
					VideoMemory[y * VGA_WIDTH + x] = VideoMemory[(y + 1) * VGA_WIDTH + x];
				}
			}
			cursor_y = VGA_HEIGHT - 1;
			for (int x = 0; x < VGA_WIDTH; x++)
			{
				VideoMemory[cursor_y * VGA_WIDTH + x] = (VideoMemory[cursor_y * VGA_WIDTH + x] & 0xFF00) | ' ';
			}
		}
	}
	move_cursor();
}

void kernel_printf_int(int value)
{
	char buffer[12];	   // Assez grand pour un entier
	itoa(value, buffer);   // Utiliser la fonction itoa pour convertir l'entier en chaîne
	kernel_printf(buffer); // Afficher la chaîne résultante
}

void itoa(int num, char *buffer)
{
	int i = 0;
	bool isNegative = false;

	if (num == 0)
	{
		buffer[i++] = '0';
		buffer[i] = '\0';
		return;
	}

	if (num < 0)
	{
		isNegative = true;
		num = -num;
	}

	while (num != 0)
	{
		buffer[i++] = (num % 10) + '0';
		num /= 10;
	}

	if (isNegative)
	{
		buffer[i++] = '-';
	}

	buffer[i] = '\0';

	for (int j = 0; j < i / 2; j++)
	{
		char temp = buffer[j];
		buffer[j] = buffer[i - j - 1];
		buffer[i - j - 1] = temp;
	}
}

extern "C" void test_message(const char *message, bool success)
{
	kernel_printf(message);
	if (success)
	{
		kernel_printf(" [ OK ]\n");
	}
	else
	{
		kernel_printf(" [ FAIL ]\n");
	}
}

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
	kernel_printf("Interrupt received: ");
	char num[3] = {(char)('0' + int_no / 10), (char)('0' + int_no % 10), '\0'};
	kernel_printf(num);
	kernel_printf("\n");
}

extern "C" void load_page_directory(uint32_t *page_directory);
extern "C" void enable_paging();

uint32_t page_directory[1024] __attribute__((aligned(4096)));
uint32_t first_page_table[1024] __attribute__((aligned(4096)));

void setup_paging()
{
	for (int i = 0; i < 1024; i++)
	{
		page_directory[i] = 0x00000002;
		first_page_table[i] = (i * 0x1000) | 3;
	}

	page_directory[0] = ((uint32_t)first_page_table) | 3;

	load_page_directory(page_directory);
	enable_paging();
	test_message("Activation de la pagination", true);
}

extern "C" uint8_t inb(uint16_t port);

extern "C" void keyboard_handler()
{
	uint8_t scancode = inb(0x60);
	kernel_printf("Key pressed\n");
}

void keyboard_install()
{
	idt_set_gate(33, (uint32_t)keyboard_handler, 0x08, 0x8E);
	test_message("Initialisation du clavier", true);
}

extern "C" void kernelMain(void *multiboot_structure, uint32_t magicnumber)
{
	clear_screen();
	kernel_printf("Initialisation du kernel...\n");

	init_memory();

	idt_install();
	keyboard_install();
	setup_paging();
	init_process_manager();

	void *ptr = allocate_memory(64);
	if (ptr)
	{
		kernel_printf("Allocated 64 bytes of memory successfully.\n");
	}
	else
	{
		kernel_printf("Failed to allocate memory.\n");
	}

	free_memory(ptr);
	kernel_printf("Memory freed successfully.\n");

	const char *process_name = "TestProcess";						 // Nom du processus
	int pid = create_process((uint32_t)process_entry, process_name); // Passer le nom

	if (pid != -1)
	{
		kernel_printf("Processus cree avec PID: ");
		kernel_printf_int(pid);
	}
	else
	{
		kernel_printf("Erreur: impossible de creer le processus.\n");
	}

	while (1)
	{
		context_switch();
	}
}
