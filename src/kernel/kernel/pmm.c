/*
 * =====================================================================================
 *
 *       Filename:  pmm.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/20/2020 07:54:52 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Friedrich Doku, 
 *   Organization:  Mutex Unlocked
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <kernel/pmm.h>
#include <kernel/serial.h>

uint32_t endkernel;
//Aligned version
uint32_t endkerneladdr;

uint32_t bitmap[BITMAP_SIZE];

void set_page_reserved(physaddr_t addr, bool set);

void pmm_init(void){
    for(int i = 0; i < BITMAP_SIZE; i++){
        bitmap[i] = 0;
    }
    if(!endkerneladdr){
        // Align page (Set last 12 bits to 0)
        endkerneladdr = (uint32_t) BLOCK_ALIGN((uint32_t)(bitmap + BITMAP_SIZE));
    }
    
    qemu_printf("END KERNEL ADDR 0x%p ", (void*) endkerneladdr);
    for(uint32_t i = 0; i < endkerneladdr; i+=PAGE_SIZE){
        set_page_reserved(i, true);
    }
}



void set_page_reserved(physaddr_t addr, bool set){
    if(!ISSET(addr)) {
        SETBIT(addr);
    }else{
        CLEARBIT(addr);
    }
}

physaddr_t kalloc_page_frame_at(physaddr_t addr){
    addr=addr&0xFFFFF000; //Align
    if(!ISSET(addr)){
        set_page_reserved(addr, true);
        return (physaddr_t) addr;
    }else{
        qemu_printf("Cannot Allocate Page Frame\n");
    }
    return 0;
}

physaddr_t kalloc_page_frame(void){
    uint32_t i;
    for(i = 0; i < 8; i++){
        if(!ISSET(i)){
            SETBIT(i);
            return (physaddr_t) endkerneladdr + (i*0x1000);
        }
    }
    qemu_printf("Failed to allocate page frame: out of mem!\n");
    return (physaddr_t)-1;
}

void kfree_page_frame(physaddr_t addr){
    addr = addr - endkerneladdr;
    if(addr == 0){
        uint32_t idx = (uint32_t)addr;
        CLEARBIT(idx);
    }else{
        // Find the index
        uint32_t idx = ((uint32_t) addr)/0x1000;
        CLEARBIT(idx);
    }
}
