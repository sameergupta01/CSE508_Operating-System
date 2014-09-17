#ifndef _STDIO_H
#define _STDIO_H

#include <defs.h>

int printf(const char *format, ...);
int print_cx_cy(int row, int col, const char *format, ...);
int scanf(const char *format, ...);
int puts(const char * string);
int puts_cx_cy(const char *string);
int putchar(int ch);
int putchar_cx_cy(int ch);

void clrscr();
char * itoa(uint64_t value, char *str, uint64_t base);
int atoi(char *p);
int octal_decimal(int n);

void *memcpy(void *dest, const void *src, size_t count);
void *memset(void *dest, char val, size_t count);
size_t strlen(const char *str);

void remap_printf();

#endif
