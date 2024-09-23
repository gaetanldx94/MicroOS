#include "includes/paging.h"
#include "includes/video.h"
#include <cstdint>

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