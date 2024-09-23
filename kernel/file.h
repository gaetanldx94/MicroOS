#ifndef FILE_H
#define FILE_H

#include <cstddef>

#define MAX_FILES 10

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

extern Directory root_directory;

bool create_file(Directory &dir, const char *filename, const char *content);
void display_file_content(const Directory &dir, const char *filename);
bool remove_file(Directory &dir, const char *filename);

#endif
