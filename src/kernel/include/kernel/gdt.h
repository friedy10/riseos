#ifndef GDT_H
#define GDT_H

#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/pmm.h>
#include <kernel/serial.h>
#include <kernel/vmm.h>
#include <kernel/liballoc.h>
//#include <kernel/idt.h>


// GDT entry
struct gdt_entry {
	unsigned short limit_low;
	unsigned short base_low;
	unsigned char  base_middle;
	unsigned char  access;
	unsigned char  granularity;
	unsigned char  base_high;
} __attribute((packed));


struct gdt_ptr {
	unsigned short limit;
	unsigned int   base;
} __attribute__((packed));

typedef struct gdt_entry gdt_entry;
typedef struct gdt_ptr	 gdt_ptr;

/* Reload new segment registers */
//extern void gdt_flush();

void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);
void gdt_install();

extern void gdt_flush(gdt_ptr* ptr);

#endif
