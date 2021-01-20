/*****************************************************************************
 *                                                                            *
 * DFU/SD/SDHC Bootloader for LPC17xx                                         *
 *                                                                            *
 * by Triffid Hunter                                                          *
 *                                                                            *
 *                                                                            *
 * This firmware is Copyright (C) 2009-2010 Michael Moon aka Triffid_Hunter   *
 *                                                                            *
 * This program is free software; you can redistribute it and/or modify       *
 * it under the terms of the GNU General Public License as published by       *
 * the Free Software Foundation; either version 2 of the License, or          *
 * (at your option) any later version.                                        *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * along with this program; if not, write to the Free Software                *
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA *
 *                                                                            *
 *****************************************************************************/

#include "min-printf.h"

#undef printf

int _write(int, const char *, int);
// int printf(const char *format, ...) __attribute__  ((__format__ (__printf__, 1, 2))));

#include <stdint.h>

int strlen(const char *str)
{
	int r;
	for (r = 0; str[r]; r++);
	return r;
}

int _puts(int fd, const char *str)
{
	int length = strlen(str);
	_write(fd, str, length);
	return length;
}

int bufp;
char printf_buf[12];

int _uint_write(uint32_t i)
{
	uint32_t j;
	uint32_t k;
// 	char c;
	int length;
	for(
		length = 0, k = 1000000000;
		k;
		k /= 10
	)
	{
		if ((i >= k) || (k == 1) || length)
		{
			j = i / k;
// 			c = j + '0';
// 			_write(0, &c, 1);
			printf_buf[bufp++] = j + '0';
			i -= j * k;
			length++;
		}
	}
	printf_buf[bufp] = 0;
	return length;
}

int _int_write(int32_t i)
{
	int length = 0;
	if (i < 0)
	{
// 		_write(0, "-", 1);
		printf_buf[bufp++] = '-';
		length++;
		i = -i;
// 		if (min_length)
// 			min_length--;
	}
	length += _uint_write((uint32_t) i);
// 	this is done in _uint_write
// 	printf_buf[bufp++] = 0;
	return length;
}

int _hex_write(uint32_t i)
{
	int length = 0;
	int q;
	const char *alpha = "0123456789ABCDEF";
	if (i) {
		for (q = 0; i && ((i & 0xF0000000) == 0); i <<= 4, q++);
		for(
			length = 0;
			i || (q < 8);
			i <<= 4, length++, q++
		)
		{
			printf_buf[bufp++] = alpha[i >> 28];
		}
	}
	else {
		printf_buf[bufp++] = '0';
	}
	printf_buf[bufp] = 0;
	return length;
}

void print0s(int num)
{
	if (num < 1)
		return;
	for (;num;num--)
		_write(0, "0", 1);
}

int vfprintf(int fd, const char *format, va_list args)
{
// 	va_list args;
// 	va_start(args, format);

	int i = 0;
	char c = 1, j = 0;

	int length = 0;
// 	int min_length = 0;

	while ((c = format[i++]))
	{
		if (j)
		{
			bufp = 0;
			switch (c)
			{
				case 's':
				{
					length += _puts(fd, va_arg(args, const char *));
					j = 0;
					break;
				}
				case 'l':
					if (j == 4)
						j = 8;
					else
						j = 4;
					break;
				case 'u':
					if (j <= 4)
						_uint_write(va_arg(args, uint32_t));
					j = 0;
					break;
				case 'd':
					if (j <= 4)
						_int_write(va_arg(args, int32_t));
					j = 0;
					break;
				case 'c':
					c = va_arg(args, int);
					_write(fd, &c, 1);
					length++;
					j = 0;
					break;
				case 'x':
					if (j <= 4)
						_hex_write(va_arg(args, uint32_t));
					j = 0;
					break;
				case 'p':
					_write(0, "0x", 2);
					_hex_write(va_arg(args, uint32_t));
					j = 0;
					break;
				default:
					printf_buf[0] = '%';
					printf_buf[1] = c;
					printf_buf[2] = 0;
					bufp = 2;
					j = 0;
					break;
			}
			if (bufp)
			{
				_write(fd, printf_buf, bufp);
				length += bufp;
				bufp = 0;
			}
		}
		else
		{
			if (c == '%')
				j = 2;
			else
			{
				_write(fd, &c, 1);
				length++;
			}
		}
	}
// 	va_end(args);
	return length;
}

int fprintf(int fd, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	int r = vfprintf(fd, format, args);
	va_end(args);
	return r;
}

#undef printf
int printf(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	int r = vfprintf(0, format, args);
	va_end(args);
	return r;
}
