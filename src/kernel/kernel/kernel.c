#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/pmm.h>
#include <kernel/serial.h>
#include <kernel/vmm.h>
#include <kernel/liballoc.h>
#include <kernel/spinlock.h>


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

    spinlock lock = 0;
    qemu_printf("Testing Spin Lock\n");
    spin_lock(&lock);

    qemu_printf("COOL COOL \n");
    spin_unlock(&lock);

    qemu_printf("Testing Heap...\n");
    
    unsigned char* arr = (unsigned char*) malloc(sizeof(char) * 10);

    for(int i = 0; i < 10; i++){
        arr[i] = "A";
    }

    for(int i = 0; i < 10; i++){
        qemu_printf("This is cool %s\n", arr[i]);
    }

    free(arr);


    qemu_printf("WORKS\n");
}
