#include "includes/file.h"
#include "includes/string_utils.h"
#include "includes/video.h"

Directory root_directory = {0};

bool create_file(Directory &dir, const char *filename, const char *content)
{
	if (dir.file_count >= MAX_FILES)
	{
		kernel_printf("Erreur : Nombre maximal de fichiers atteint.\n");
		return false;
	}

	File &file = dir.files[dir.file_count];
	my_strncpy(file.name, filename, sizeof(file.name) - 1);
	file.name[sizeof(file.name) - 1] = '\0';
	my_strncpy(file.content, content, sizeof(file.content) - 1);
	file.content[sizeof(file.content) - 1] = '\0';
	file.size = my_strlen(content);
	dir.file_count++;
	return true;
}

void display_file_content(const Directory &dir, const char *filename)
{
	bool file_found = false;

	for (size_t i = 0; i < dir.file_count; i++)
	{
		if (my_strcmp(dir.files[i].name, filename) == 0)
		{
			file_found = true;
			kernel_printf("\n%s\n", dir.files[i].content);

			break;
		}
	}

	if (!file_found)
	{
		kernel_printf("\nFichier %s non trouve.\n", filename);
	}
}

bool remove_file(Directory &dir, const char *filename)
{
	for (size_t i = 0; i < dir.file_count; i++)
	{
		if (my_strcmp(dir.files[i].name, filename) == 0)
		{
			for (size_t j = i; j < dir.file_count - 1; j++)
			{
				dir.files[j] = dir.files[j + 1];
			}
			dir.file_count--;
			kernel_printf("\nFichier %s supprime avec succes.\n", filename);
			return true;
		}
	}

	kernel_printf("\nErreur: Fichier %s non trouve.\n", filename);
	return false;
}
