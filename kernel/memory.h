#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>
#include <cstddef>

struct MemoryBlock
{
	size_t size;
	bool free;
	MemoryBlock *next;
};

extern "C"
{
	void init_memory();
	void *allocate_memory(size_t size);
	void free_memory(void *ptr);
}

#endif