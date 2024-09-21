#include "file.h"
#include "string_utils.h"
#include "kernel.h"

bool create_file(Directory &dir, const char *filename, const char *content)
{
	if (dir.file_count >= MAX_FILES)
	{
		kernel_printf("Erreur : Nombre maximal de fichiers atteint.\n");
		return false;
	}

	File &file = dir.files[dir.file_count];
	my_strncpy(file.name, filename, sizeof(file.name) - 1);
	file.name[sizeof(file.name) - 1] = '\0'; // Assurez-vous que le nom est null-terminé
	my_strncpy(file.content, content, sizeof(file.content) - 1);
	file.content[sizeof(file.content) - 1] = '\0'; // Assurez-vous que le contenu est null-terminé
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
			kernel_printf("\nContenu de %s :\n%s\n", filename, dir.files[i].content);

			break;
		}
	}

	if (!file_found)
	{
		kernel_printf("\nFichier %s non trouve.\n", filename);
	}
}
