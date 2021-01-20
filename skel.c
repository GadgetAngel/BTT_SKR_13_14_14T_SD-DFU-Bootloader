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

#include <stdlib.h>
#include <stdio.h>

#define WEAK __attribute__ ((weak))

#include "LPC17xx.h"

void WEAK _exit(int i) {
	while (1);
}

int WEAK _kill(int pid, int signal) {
	return 0;
}

int WEAK _getpid() {
	return 0;
}

int WEAK __aeabi_atexit(void *object, void (*destructor)(void *), void *dso_handle) {
	return 0;
}

int WEAK _write(int fd, uint8_t *buf, size_t buflen) {
// 		_dbg_init();
// 		dbg->send((uint8_t *) buf, buflen);
	return buflen;
}

int WEAK _close(int fd) {
	return 0;
}

int WEAK _lseek(int file, int ptr, int dir) {
	return ptr;
}

int WEAK _read(int file, char *buf, int len) {
// 		_dbg_init();
// 		return dbg->recv((uint8_t *) buf, len);
	return 0;
}

void* WEAK _sbrk(int incr) {

	extern char _ebss; // Defined by the linker
	static char *heap_end;
	char *prev_heap_end;

	if (heap_end == 0) {
		heap_end = &_ebss;
	}
	prev_heap_end = heap_end;

	char * stack = (char*) __get_MSP();
	if (heap_end + incr >  stack)
	{
		//_write (STDERR_FILENO, "Heap and stack collision\n", 25);
		//errno = ENOMEM;
		return  (void *) -1;
		//abort ();
	}

	heap_end += incr;
	return prev_heap_end;
}

int WEAK _fstat(int file, void *st) {
	return 0;
}

int WEAK _isatty(int fd) {
	if (fd <= 2)
		return 1;
	return 0;
}

void __aeabi_unwind_cpp_pr0(void){}
void __libc_init_array(void){}
int atexit(void(*f)(void)){ return 0; }
