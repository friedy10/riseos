#include <kernel/gdt.h>


void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran){

    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F);

    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access = access;
}

void gdt_install(){

    // Set the gdt pointer and limit
    gp.limit = (sizeof(gdt_entry) *3) -1;
    gp.base = &gdt;

    // Set NULL descriptor (not used by the processor)
    gdt_set_gate(0, 0, 0, 0, 0);

    // The second entry is the code segment. 4 GB limit
    // 4KB granularity, 32 bit opcodes
    gdt_set_gate(1,0, 0xFFFFFFFF, 0x9A, 0xCF);

    // data segment
    gdt_set_gate(2,0, 0xFFFFFFFF, 0x9A, 0xCF);

    // flush out old gdt and update
    gdt_flush();
}
