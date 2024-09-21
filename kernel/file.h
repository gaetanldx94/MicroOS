#ifndef FILE_H
#define FILE_H

#include <cstddef> // Pour size_t

#define MAX_FILES 10 // Limite du nombre de fichiers dans un répertoire

struct File
{
	char name[50];
	char content[256];
	size_t size;
};

struct Directory
{
	size_t file_count;
	File files[MAX_FILES];
};

// Déclaration externe de root_directory
extern Directory root_directory;

// Déclarations de fonctions
bool create_file(Directory &dir, const char *filename, const char *content);
void display_file_content(const Directory &dir, const char *filename);

#endif // FILE_H
