#include "includes/string_utils.h"
#include <cstdarg>

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
	dest[n - 1] = '\0';
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

int my_strncmp(const char *str1, const char *str2, size_t n)
{
	while (n && (*str1 || *str2))
	{
		if (*str1 != *str2)
		{
			return (unsigned char)(*str1) - (unsigned char)(*str2);
		}
		str1++;
		str2++;
		n--;
	}
	return 0;
}

const char *my_strchr(const char *str, char c)
{
	while (*str != '\0')
	{
		if (*str == c)
		{
			return str;
		}
		str++;
	}
	return nullptr;
}

void my_snprintf(char *buffer, size_t size, const char *format, va_list args)
{
	size_t written = 0;
	for (const char *p = format; *p && written < size - 1; p++)
	{
		if (*p == '%')
		{
			p++;
			if (*p == 's')
			{
				const char *str = va_arg(args, const char *);
				while (*str && written < size - 1)
				{
					buffer[written++] = *str++;
				}
			}
			else if (*p == 'c')
			{
				char c = (char)va_arg(args, int);
				buffer[written++] = c;
			}
			else
			{
				buffer[written++] = *p;
			}
		}
		else
		{
			buffer[written++] = *p;
		}
	}
	buffer[written] = '\0';
}
