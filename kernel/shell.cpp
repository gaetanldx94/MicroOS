#include "includes/shell.h"
#include "includes/string_utils.h"
#include "includes/file.h"
#include "includes/video.h"
#include "includes/keyboard.h"
#include "includes/memory.h"

void read_command(char *buffer, size_t size);
void execute_command(const char *command);
void list_files();

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

void parse_cat_command(const char *command, char *filename)
{
	const char *start = command + 4;
	while (*start != '\0' && *start != ' ')
	{
		*filename++ = *start++;
	}
	*filename = '\0';
}

inline void outw(uint16_t port, uint16_t value)
{
	asm volatile("outw %0, %1" : : "a"(value), "Nd"(port));
}

void execute_command(const char *command)
{
	if (my_strcmp(command, "shutdown") == 0)
	{
		outw(0x604, 0x2000);

		while (true)
		{
			asm volatile("hlt");
		}
	}
	else if (my_strncmp(command, "ls", 2) == 0)
	{
		list_files();
	}
	else if (my_strncmp(command, "ctd", 3) == 0)
	{
		char filename[256];
		parse_cat_command(command, filename);
		display_file_content(root_directory, filename);
	}
	else if (my_strncmp(command, "clear", 5) == 0)
	{
		clear_screen();
	}
	else if (my_strncmp(command, "cf", 2) == 0)
	{
		const char *args = command + 3;

		const char *space = my_strchr(args, ' ');
		if (!space)
		{
			space = " ";
		}

		char filename[256];
		my_strncpy(filename, args, space - args);
		filename[space - args] = '\0';

		const char *content = space + 1;

		if (!create_file(root_directory, filename, content))
		{
			kernel_printf("\nErreur: Impossible de creer le fichier %s.\n", filename);
		}
		else
		{
			kernel_printf("\nFichier %s cree avec succes.\n", filename);
		}
	}
	else if (my_strncmp(command, "rm", 2) == 0)
	{
		const char *filename = command + 3;

		if (my_strlen(filename) == 0)
		{
			kernel_printf("\nErreur: Aucun nom de fichier fourni.\n");
			return;
		}

		remove_file(root_directory, filename);
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
		kernel_printf("\n%s", root_directory.files[i].name);
	}
	kernel_printf("\n");
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

		if (c == '\b')
		{
			if (i > 0)
			{
				i--;
				buffer[i] = '\0';

				cursor_x--;
				if (cursor_x < 0)
				{
					cursor_x = VGA_WIDTH - 1;
					cursor_y--;
				}
				VideoMemory[cursor_y * VGA_WIDTH + cursor_x] = ' ' | (0x07 << 8);
			}
			continue;
		}

		buffer[i] = c;
		i++;

		kernel_printf("%c", c);
	}

	buffer[i] = '\0';
}