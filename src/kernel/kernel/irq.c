#include <kernel/irq.h>
#include <kernel/serial.h>

#include <stdio.h>
#include <kernel/tty.h>
void * irq_routines[16] = {
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0};

// installs a custom irq handler for the given IRQ
void irq_install_handler(int irq, void (*handler)(struct regs * r))
{
	irq_routines[irq] = handler;
}

// Clears handler
void irq_uninstall_handler(int irq)
{
	irq_routines[irq] = 0;
}

// IDT entry 8 is a double fault.
// Remap IRQ 0 to 15 to 32 to 47 for PIC
// devices
void irq_remap(void)
{
	outportb(0x20, 0x11);
	outportb(0xA0, 0x11);
	outportb(0x21, 0x20);
	outportb(0xA1, 0x28);
	outportb(0x21, 0x04);
	outportb(0xA1, 0x02);
	outportb(0x21, 0x01);
	outportb(0xA1, 0x01);
	outportb(0x21, 0x0);
	outportb(0xA1, 0x0);
}

// Install ISRs to IDT
void irq_install()
{
	irq_remap();

	idt_set_gate(32, (unsigned)irq0, 0x08, 0x8E);
	idt_set_gate(33, (unsigned)irq1, 0x08, 0x8E);
	idt_set_gate(34, (unsigned)irq2, 0x08, 0x8E);
	idt_set_gate(35, (unsigned)irq3, 0x08, 0x8E);
	idt_set_gate(36, (unsigned)irq4, 0x08, 0x8E);
	idt_set_gate(37, (unsigned)irq5, 0x08, 0x8E);
	idt_set_gate(38, (unsigned)irq6, 0x08, 0x8E);
	idt_set_gate(39, (unsigned)irq7, 0x08, 0x8E);
	idt_set_gate(40, (unsigned)irq8, 0x08, 0x8E);
	idt_set_gate(41, (unsigned)irq9, 0x08, 0x8E);
	idt_set_gate(42, (unsigned)irq10, 0x08, 0x8E);
	idt_set_gate(43, (unsigned)irq11, 0x08, 0x8E);
	idt_set_gate(44, (unsigned)irq12, 0x08, 0x8E);
	idt_set_gate(45, (unsigned)irq13, 0x08, 0x8E);
	idt_set_gate(46, (unsigned)irq14, 0x08, 0x8E);
	idt_set_gate(47, (unsigned)irq15, 0x08, 0x8E);
}

// fault handler for irq isrs
void irq_handler(struct regs * r)
{

    //TODO: Fix me the IRQ handler fires, but I am getting the wrong
    //number. Figure out why r->int_no is wrong.
    
    qemu_printf("FAULT HANDLER FIRED %d", r->int_no);
    // Blank funciton pointer
	void (*handler)(struct regs * r);

	// If we have a custom handler let's use taht
	handler = irq_routines[r->int_no - 32];

	if (handler) {
		handler(r);
	}

	// it's greater than 40 lets send an EOI to the slave
	if (r->int_no >= 40) {
		outportb(0xA0, 0x20);
	}

	outportb(0x20, 0x20);
}
