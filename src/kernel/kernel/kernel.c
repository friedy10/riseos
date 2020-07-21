#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/pmm.h>

void kernel_main(void) {
    init_bitmap();
    terminal_initialize();
    printf("Hello, kernel World!\n");

}
