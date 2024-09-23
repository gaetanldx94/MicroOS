#include "includes/process.h"
#include "includes/video.h"

extern "C" void *allocate_memory(std::size_t size);
const int MAX_PROCESSES = 256;
Process process_table[MAX_PROCESSES];
int current_process = -1;

void init_process_manager()
{
	for (int i = 0; i < MAX_PROCESSES; i++)
	{
		process_table[i].pid = -1;
		process_table[i].state = ProcessState::READY;
	}
}

int create_process(uint32_t entry_point, const char *process_name)
{
	for (int i = 0; i < MAX_PROCESSES; i++)
	{
		if (process_table[i].state == ProcessState::READY)
		{
			process_table[i].pid = i;
			process_table[i].stackPointer = (uint32_t)allocate_memory(4096);
			if (!process_table[i].stackPointer)
			{
				return -1;
			}
			process_table[i].programCounter = entry_point;
			process_table[i].state = ProcessState::RUNNING;
			return i;
		}
	}
	return -1;
}

void context_switch()
{
	if (current_process != -1)
	{
		process_table[current_process].state = ProcessState::READY;
	}

	do
	{
		current_process = (current_process + 1) % MAX_PROCESSES;
	} while (process_table[current_process].state != ProcessState::READY);

	process_table[current_process].state = ProcessState::RUNNING;
}

void format_process_message(char *buffer, int size, const char *process_name, uint32_t pid)
{
	if (size > 0)
	{
		int written = 0;
		while (*process_name && written < size - 1)
		{
			buffer[written++] = *process_name++;
		}
		buffer[written++] = ' ';
		buffer[written++] = 'a';
		buffer[written++] = 'v';
		buffer[written++] = 'e';
		buffer[written++] = 'c';
		buffer[written++] = ' ';
		buffer[written++] = 'P';
		buffer[written++] = 'I';
		buffer[written++] = 'D';
		buffer[written++] = ':';
		buffer[written++] = ' ';

		int pid_copy = pid;
		if (pid_copy == 0)
		{
			buffer[written++] = '0';
		}
		else
		{
			char pid_buffer[10];
			int pid_length = 0;
			while (pid_copy > 0 && pid_length < 10)
			{
				pid_buffer[pid_length++] = '0' + (pid_copy % 10);
				pid_copy /= 10;
			}
			for (int i = pid_length - 1; i >= 0; i--)
			{
				if (written < size - 1)
				{
					buffer[written++] = pid_buffer[i];
				}
			}
		}
		buffer[written] = '\0';
	}
}

extern "C" void process_entry(uint32_t pid, const char *process_name)
{
	char buffer[100];

	format_process_message(buffer, sizeof(buffer), process_name, pid);
	kernel_printf(buffer);

	while (1)
	{
	}
}
