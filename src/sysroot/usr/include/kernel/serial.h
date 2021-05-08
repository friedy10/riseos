/*
 * =====================================================================================
 *
 *       Filename:  serial.h
 *
 *    Description: Header file for serial.c 
 *
 *        Version:  1.0
 *        Created:  05/03/2021 06:09:26 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Friedrich Doku, 
 *   Organization:  Mutex Unlocked
 *
 * =====================================================================================
 */

#ifndef SERIAL_H
#define SERIAL_H

#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

void vsprintf(char * str, void (*putchar)(char), const char * format, va_list arg);
void vsprintf_helper(char * str, void (*putchar)(char), const char * format, uint32_t * pos, va_list arg);

// Receiving Data
int serial_received();
 
char read_serial();

// Sending Data
int is_transmit_empty();
 
void write_serial(char a);

// Print to QEMU
void qemu_printf(const char * s, ...);

void serial_init(void);

#endif
