#include <cstdint>

extern "C" void kernel_printf(const char *str)
{
	static unsigned short *VideoMemory = (unsigned short *)0xb8000;

	for (int i = 0; str[i] != '\0'; ++i)
	{
		VideoMemory[i] = (VideoMemory[i] & 0xFF00) | str[i];
	}
}

extern "C" void kernelMain(void *multiboot_structure, uint32_t magicnumber)
{
	kernel_printf("Hello World");
	while (1)
		;
}
