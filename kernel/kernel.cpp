#include <cstdint>
#include <cstdio>
#include <cstdarg>

#include "memory.h"
#include "process.h"
#include "file.h"
#include "string_utils.h"

Directory root_directory = {.file_count = 0};

void itoa(int num, char *buffer);

const int VGA_WIDTH = 80;
const int VGA_HEIGHT = 25;
unsigned short *VideoMemory = (unsigned short *)0xb8000;
int cursor_x = 0;
int cursor_y = 0;

extern "C" void init_process_manager();
extern "C" void context_switch();
extern "C" void process_entry(uint32_t pid, const char *process_name);

void read_command(char *buffer, size_t size);
void execute_command(const char *command);
void list_files();

extern "C" void kernel_printf(const char *format, ...);
extern "C" uint8_t inb(uint16_t port);

char get_char();

void shell()
{
	char command[256];

	while (true)
	{
		kernel_printf("MicroOS> ");
		read_command(command, sizeof(command));
		execute_command(command);
	}
}

bool key_pressed[256] = {false};

char get_char()
{
	uint8_t scancode = inb(0x60);

	if (scancode & 0x80)
	{
		key_pressed[scancode & 0x7F] = false;
		return '\0';
	}

	if (key_pressed[scancode])
	{
		return '\0';
	}

	key_pressed[scancode] = true;

	switch (scancode)
	{
	case 0x10:
		return 'a'; // A
	case 0x11:
		return 'z'; // Z
	case 0x12:
		return 'e'; // E
	case 0x13:
		return 'r'; // R
	case 0x14:
		return 't'; // T
	case 0x15:
		return 'y'; // Y
	case 0x16:
		return 'u'; // U
	case 0x17:
		return 'i'; // I
	case 0x18:
		return 'o'; // O
	case 0x19:
		return 'p'; // P
	case 0x1E:
		return 'q'; // Q
	case 0x1F:
		return 's'; // S
	case 0x20:
		return 'd'; // D
	case 0x21:
		return 'f'; // F
	case 0x22:
		return 'g'; // G
	case 0x23:
		return 'h'; // H
	case 0x24:
		return 'j'; // J
	case 0x25:
		return 'k'; // K
	case 0x26:
		return 'l'; // L
	case 0x27:
		return 'm'; // M
	case 0x2C:
		return 'w'; // W
	case 0x2D:
		return 'x'; // X
	case 0x2E:
		return 'c'; // C
	case 0x2F:
		return 'v'; // V
	case 0x30:
		return 'b'; // B
	case 0x31:
		return 'n'; // B
	case 0x39:
		return ' '; // Espace
	case 0x1C:
		return '\n'; // Entrée
	default:
		return '\0'; // Retourne un caractère nul si non reconnu
	}
}

void read_command(char *buffer, size_t size)
{
	size_t i = 0;
	char c;

	while (i < size - 1)
	{
		c = get_char();

		if (c == '\0')
		{
			continue;
		}

		if (c == '\n')
		{
			break;
		}

		buffer[i] = c;
		kernel_printf("%c", c);

		i++;
	}
	buffer[i] = '\0';
}

void parse_cat_command(const char *command, char *filename)
{
	const char *start = command + 4;
	while (*start != '\0' && *start != ' ')
	{
		*filename++ = *start++;
	}
	*filename = '\0';
}

void execute_command(const char *command)
{
	if (my_strcmp(command, "exit") == 0)
	{
		kernel_printf("Sortie du shell.\n");
	}
	else if (my_strncmp(command, "ls", 2) == 0)
	{
		list_files();
	}
	else if (my_strncmp(command, "cat", 3) == 0)
	{
		char filename[256];
		parse_cat_command(command, filename);
		display_file_content(root_directory, filename);
	}
	else
	{
		kernel_printf("\nCommande non reconnue : %s\n", command);
	}
}

void list_files()
{
	for (size_t i = 0; i < root_directory.file_count; i++)
	{
		kernel_printf("\n%s\n", root_directory.files[i].name);
	}
}

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

extern "C" void kernel_printf(const char *format, ...)
{
	char buffer[256];
	va_list args;
	va_start(args, format);

	my_snprintf(buffer, sizeof(buffer), format, args);
	va_end(args);

	for (int i = 0; buffer[i] != '\0'; ++i)
	{
		char c = buffer[i];
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
	char buffer[12];
	itoa(value, buffer);
	kernel_printf(buffer);
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
	kernel_printf("Interruption reçue: ");
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
		kernel_printf("64 octets de memoire alloues avec succes.\n");
	}
	else
	{
		kernel_printf("Echec de l'allocation de memoire.\n");
	}

	free_memory(ptr);
	kernel_printf("Memoire liberee avec succes.\n");

	const char *process_name = "TestProcess";
	int pid = create_process((uint32_t)process_entry, process_name);

	if (pid != -1)
	{
		kernel_printf("Processus cree avec PID: ");
		kernel_printf_int(pid);
	}
	else
	{
		kernel_printf("Erreur: impossible de creer le processus.\n");
	}

	if (create_file(root_directory, "monfichier", "Ceci est un test de fichier."))
	{
		kernel_printf("\nFichier cree avec succes.\n");
	}
	else
	{
		kernel_printf("Échec de la création du fichier.\n");
	}

	display_file_content(root_directory, "monfichier");

	kernel_printf("\n");

	shell();

	while (1)
	{
		context_switch();
	}
}
