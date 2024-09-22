#ifndef PROCESS_H
#define PROCESS_H

#include <cstdint>

enum class ProcessState
{
	FREE,
	READY,
	RUNNING,
	BLOCKED,
	TERMINATED
};

struct Process
{
	uint32_t pid;
	ProcessState state;
	uint32_t stackPointer;
	uint32_t programCounter;
};

extern "C"
{
	void init_process_manager();
	int create_process(uint32_t entry_point, const char *process_name);
	void context_switch();
	void terminate_process(uint32_t pid);
	void process_entry(uint32_t pid, const char *process_name);
}

#endif
