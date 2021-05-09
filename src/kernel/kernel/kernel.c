#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/pmm.h>
#include <kernel/serial.h>
#include <kernel/vmm.h>


void kernel_main(void) {

    pmm_init();
    terminal_initialize();
    serial_init();
    paging_init();
    printf("Hello, kernel World!\n");
    
    printf("Testing memeory manager\n");
    
    qemu_printf("HELLO WORLD FROM QEMU\n");
    for(int i = 0; i < 8; i++){
        physaddr_t frame = kalloc_page_frame();
        qemu_printf("Loaded Page Frame At 0x%p \n", (void*) frame);
    }

}
