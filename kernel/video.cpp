#include "includes/video.h"
#include "includes/string_utils.h"
#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>

int cursor_x = 0;
int cursor_y = 0;
const int VGA_WIDTH = 80;
const int VGA_HEIGHT = 25;
uint16_t *VideoMemory = (uint16_t *)0xB8000;

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

extern "C" void test_message(const char *message, bool success)
{
	if (success)
	{
		kernel_printf("\n[ OK ] ");
	}
	else
	{
		kernel_printf("\n[ FAIL ] ");
	}
	kernel_printf(message);
}
