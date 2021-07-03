# Declare constants for the multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set FLAGS,    ALIGN | MEMINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Declare a header as in the Multiboot Standard.
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Reserve a stack for the initial thread.
.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

.section .text

# Load the page directory
.global load_page_directory
.type load_page_directory, @function
load_page_directory:
	push %ebp
	mov %esp, %ebp
	mov 8(%esp), %eax
	mov %eax, %cr3
	mov %ebp, %esp
	pop %ebp
	ret

# Enable paging
.global enable_paging
.type enable_paging, @function
enable_paging:
    push %ebp
    mov %esp, %ebp
	mov %cr0, %eax
	or  $0x80000000, %eax
	mov %eax, %cr0
	mov %ebp, %esp
	pop %ebp
	ret
    
.global idt_load
.type idt_load,@function
idt_load:
    push %ebp
    mov %esp, %ebp
    mov 8(%esp), %eax
    lidt (%eax)
    mov %ebp, %esp
    pop %ebp
    ret

# ISRs
.global isr0
.global isr1
.global isr2
.global isr3
.global isr4
.global isr5
.global isr6
.global isr7
.global isr8
.global isr9
.global isr10
.global isr11
.global isr12
.global isr13
.global isr14
.global isr15
.global isr16
.global isr17
.global isr18
.global isr19
.global isr20
.global isr21
.global isr22
.global isr23
.global isr24
.global isr25
.global isr26
.global isr27
.global isr28
.global isr29
.global isr30
.global isr31

#  0: Divide By Zero Exception
.type isr0, @function
isr0:
    cli
    push   0
    push   0
    jmp isr_common_stub

#  1: Debug Exception
.type isr1, @function
isr1:
    cli
    push   0
    push   1
    jmp isr_common_stub

#  2: Non Maskable Interrupt Exception
.type isr2, @function
isr2:
    cli
    push   0
    push   2
    jmp isr_common_stub

#  3: Int 3 Exception
.type isr3, @function
isr3:
    cli
    push   0
    push   3
    jmp isr_common_stub

#  4: INTO Exception
.type isr4, @function
isr4:
    cli
    push   0
    push   4
    jmp isr_common_stub

#  5: Out of Bounds Exception
.type isr5, @function
isr5:
    cli
    push   0
    push   5
    jmp isr_common_stub

#  6: Invalid Opcode Exception
.type isr6, @function
isr6:
    cli
    push   0
    push   6
    jmp isr_common_stub

#  7: Coprocessor Not Available Exception
.type isr7, @function
isr7:
    cli
    push   0
    push   7
    jmp isr_common_stub

#  8: Double Fault Exception (With Error Code!)
.type isr8, @function
isr8:
    cli
    push   8
    jmp isr_common_stub

#  9: Coprocessor Segment Overrun Exception
.type isr9, @function
isr9:
    cli
    push   0
    push   9
    jmp isr_common_stub

# 10: Bad TSS Exception (With Error Code!)
.type isr10, @function
isr10:
    cli
    push   10
    jmp isr_common_stub

# 11: Segment Not Present Exception (With Error Code!)
.type isr11, @function
isr11:
    cli
    push   11
    jmp isr_common_stub

# 12: Stack Fault Exception (With Error Code!)
.type isr12, @function
isr12:
    cli
    push   12
    jmp isr_common_stub

# 13: General Protection Fault Exception (With Error Code!)
.type isr13, @function
isr13:
    cli
    push   13
    jmp isr_common_stub

# 14: Page Fault Exception (With Error Code!)
.type isr14, @function
isr14:
    cli
    push   14
    jmp isr_common_stub

# 15: Reserved Exception
.type isr15, @function
isr15:
    cli
    push   0
    push   15
    jmp isr_common_stub

# 16: Floating Point Exception
.type isr16, @function
isr16:
    cli
    push   0
    push   16
    jmp isr_common_stub

# 17: Alignment Check Exception
.type isr17, @function
isr17:
    cli
    push   0
    push   17
    jmp isr_common_stub

# 18: Machine Check Exception
.type isr18, @function
isr18:
    cli
    push   0
    push   18
    jmp isr_common_stub

# 19: Reserved
.type isr19, @function
isr19:
    cli
    push   0
    push   19
    jmp isr_common_stub

# 20: Reserved
.type isr20, @function
isr20:
    cli
    push   0
    push   20
    jmp isr_common_stub

# 21: Reserved
.type isr21, @function
isr21:
    cli
    push   0
    push   21
    jmp isr_common_stub

# 22: Reserved
.type isr22, @function
isr22:
    cli
    push   0
    push   22
    jmp isr_common_stub

# 23: Reserved
.type isr23, @function
isr23:
    cli
    push   0
    push   23
    jmp isr_common_stub

# 24: Reserved
.type isr24, @function
isr24:
    cli
    push   0
    push   24
    jmp isr_common_stub

# 25: Reserved
.type isr25, @function
isr25:
    cli
    push   0
    push   25
    jmp isr_common_stub

# 26: Reserved
.type isr26, @function
isr26:
    cli
    push   0
    push   26
    jmp isr_common_stub

# 27: Reserved
.type isr27, @function
isr27:
    cli
    push   0
    push   27
    jmp isr_common_stub

# 28: Reserved
.type isr28, @function
isr28:
    cli
    push   0
    push   28
    jmp isr_common_stub

# 29: Reserved
.type isr29, @function
isr29:
    cli
    push   0
    push   29
    jmp isr_common_stub

# 30: Reserved
.type isr30, @function
isr30:
    cli
    push   0
    push   30
    jmp isr_common_stub

# 31: Reserved
.type isr31, @function
isr31:
    cli
    push   0
    push   31
    jmp isr_common_stub

# We call a C function in here. We need to let the assembler know
# that '_fault_handler' exists in another file
.global fault_handler

# This is our common ISR stub. It saves the processor state, sets
# up for kernel mode segments, calls the C-level fault handler,
# and finally restores the stack frame.
isr_common_stub:
    pusha
    push %ds
    push %es
    push %fs
    push %gs
    mov $0x10, %ax  # Set data segments to kernel mode 0x10
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %esp, %eax
    push %eax
    mov fault_handler, %eax
    call *%eax
    pop %eax
    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa
    add $8, %esp
    iret

.global irq0
.global irq1
.global irq2
.global irq3
.global irq4
.global irq5
.global irq6
.global irq7
.global irq8
.global irq9
.global irq10
.global irq11
.global irq12
.global irq13
.global irq14
.global irq15

# 32: IRQ0
.type irq0, @function
irq0:
    cli
    push   0
    push   32
    jmp irq_common_stub

# 33: IRQ1
.type irq1, @function
irq1:
    cli
    push   0
    push   33
    jmp irq_common_stub

# 34: IRQ2
.type irq2, @function
irq2:
    cli
    push   0
    push   34
    jmp irq_common_stub

# 35: IRQ3
.type irq3, @function
irq3:
    cli
    push   0
    push   35
    jmp irq_common_stub

# 36: IRQ4
.type irq4, @function
irq4:
    cli
    push   0
    push   36
    jmp irq_common_stub

# 37: IRQ5
.type irq5, @function
irq5:
    cli
    push   0
    push   37
    jmp irq_common_stub

# 38: IRQ6
.type irq6, @function
irq6:
    cli
    push   0
    push   38
    jmp irq_common_stub

# 39: IRQ7
.type irq7, @function
irq7:
    cli
    push   0
    push   39
    jmp irq_common_stub

# 40: IRQ8
.type irq8, @function
irq8:
    cli
    push   0
    push   40
    jmp irq_common_stub

# 41: IRQ9
.type irq9, @function
irq9:
    cli
    push   0
    push   41
    jmp irq_common_stub

# 42: IRQ10
.type irq10, @function
irq10:
    cli
    push   0
    push   42
    jmp irq_common_stub

# 43: IRQ11
.type irq11, @function
irq11:
    cli
    push   0
    push   43
    jmp irq_common_stub

# 44: IRQ12
.type irq12, @function
irq12:
    cli
    push   0
    push   44
    jmp irq_common_stub

# 45: IRQ13
.type irq13, @function
irq13:
    cli
    push   0
    push   45
    jmp irq_common_stub

# 46: IRQ14
.type irq14, @function
irq14:
    cli
    push   0
    push   46
    jmp irq_common_stub

# 47: IRQ15
.type irq15, @function
irq15:
    cli
    push   0
    push   47
    jmp irq_common_stub

.global irq_handler

irq_common_stub:
    pusha
    push %ds
    push %es
    push %fs
    push %gs

    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %esp, %eax

    push %eax
    mov irq_handler, %eax
    call *%eax
    pop %eax

    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa
    add $8, %esp
    iret

# Tell the processor where the new GDT exist
.global gdt_flush
.type gdt_flush, @function
gdt_flush:
    lgdt (gp)
    mov $0x10, %ax      # 0x10 is the offset in the GDT to our kernel data segment
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
   # mov %ax, %ss
    jmp $0x08,$flush2     # 0x08 offsert for code segment and execute flush

flush2:
    ret                 # Goes back to C code          


# The kernel entry point.
.global _start
.type _start, @function
_start:
	movl $stack_top, %esp

	# Call the global constructors.
	call _init

	# Transfer control to the main kernel.
	call kernel_main

	# Hang if kernel_main unexpectedly returns.
	cli
1:	hlt
	jmp 1b
.size _start, . - _start
