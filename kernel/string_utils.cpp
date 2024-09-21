#include "string_utils.h"
#include <cstdarg> // Ajouté pour va_list, va_start, va_end

size_t my_strlen(const char *str)
{
	size_t length = 0;
	while (str[length] != '\0')
	{
		length++;
	}
	return length;
}

void my_strncpy(char *dest, const char *src, size_t n)
{
	for (size_t i = 0; i < n && src[i] != '\0'; i++)
	{
		dest[i] = src[i];
	}
	dest[n - 1] = '\0'; // Assurez-vous que le destinataire est null-terminé
}

int my_strcmp(const char *str1, const char *str2)
{
	while (*str1 && (*str1 == *str2))
	{
		str1++;
		str2++;
	}
	return *(unsigned char *)str1 - *(unsigned char *)str2;
}

void my_snprintf(char *buffer, size_t size, const char *format, va_list args)
{
	size_t written = 0;
	for (const char *p = format; *p && written < size - 1; p++)
	{
		if (*p == '%')
		{
			p++; // Passe au caractère suivant
			if (*p == 's')
			{
				const char *str = va_arg(args, const char *);
				while (*str && written < size - 1)
				{
					buffer[written++] = *str++;
				}
			}
			else
			{
				buffer[written++] = *p; // Ajoute le caractère de format
			}
		}
		else
		{
			buffer[written++] = *p; // Ajoute le caractère normal
		}
	}
	buffer[written] = '\0'; // Assurez-vous que le buffer est null-terminé
}
