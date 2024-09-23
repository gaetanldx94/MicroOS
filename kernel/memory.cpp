#include "includes/memory.h"

MemoryBlock *memory_list = nullptr;

void init_memory()
{
	memory_list = (MemoryBlock *)0x100000;
	memory_list->size = 0x100000 - sizeof(MemoryBlock);
	memory_list->free = true;
	memory_list->next = nullptr;
}

void *allocate_memory(size_t size)
{
	MemoryBlock *current = memory_list;

	while (current)
	{
		if (current->free && current->size >= size)
		{
			current->free = false;

			if (current->size > size + sizeof(MemoryBlock))
			{
				MemoryBlock *next_block = (MemoryBlock *)((uint8_t *)current + sizeof(MemoryBlock) + size);
				next_block->size = current->size - size - sizeof(MemoryBlock);
				next_block->free = true;
				next_block->next = current->next;

				current->size = size;
				current->next = next_block;
			}
			return (void *)(current + 1);
		}
		current = current->next;
	}
	return nullptr;
}

void free_memory(void *ptr)
{
	if (!ptr)
		return;

	MemoryBlock *block = (MemoryBlock *)ptr - 1;
	block->free = true;

	MemoryBlock *current = memory_list;
	while (current)
	{
		if (current->free && current->next && current->next->free)
		{
			current->size += sizeof(MemoryBlock) + current->next->size;
			current->next = current->next->next;
		}
		else
		{
			current = current->next;
		}
	}
}
