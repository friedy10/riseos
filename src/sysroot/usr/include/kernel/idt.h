#ifndef IDT_H
#define IDT_H

#include <stdio.h>

// https://wiki.osdev.org/Interrupt_Descriptor_Table
struct idt_entry{
    unsigned short base_lo;
    unsigned short sel;
    unsigned char always0;
    unsigned char flags;
    unsigned short base_hi;
} __attribute__((packed));

struct idt_ptr{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

struct idt_entry idt[256];
struct idt_ptr idtp;

typedef struct idt_entry idt_entry;
typedef struct idt_prt idt_prt;

extern void idt_load(idt_ptr *ptr);


void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel);

void idt_install();

#endif
