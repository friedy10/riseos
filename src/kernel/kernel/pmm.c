/*
 * =====================================================================================
 *
 *       Filename:  paging.c
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

#define INDEX_FROM_BIT(a) (a >> 5)
#define OFFSET_FROM_BIT(a) (a%(8*4))

uint32_t endkernel;
uint32_t endkerneladdr;

void set_page_reserved(physaddr_t addr, bool set);

void init_bitmap(void){
    for(int i = 0; i < BITMAP_SIZE; i++){
        bitmap[i] = 0;
    }
    if(!endkerneladdr){
        // Align page
        endkerneladdr = (((uint32_t)&endkernel & 0xFFFFF000) + PAGE_SIZE); 
    }
    for(uint32_t i = 0; i < endkerneladdr; i+=PAGE_SIZE){
        set_page_reserved(i, true);
    }
}


bool is_free_page(physaddr_t addr){
    // Calculate the offset
    unsigned i = INDEX_FROM_BIT(addr);
    unsigned offset = OFFSET_FROM_BIT(addr); 
    return (bitmap[i] & (0x1 << offset));
}

void set_page_reserved(physaddr_t addr, bool set){
    unsigned i = INDEX_FROM_BIT(addr);
    unsigned offset = OFFSET_FROM_BIT(addr);
    if(set) {
        bitmap[i] |= (0x1 << offset);
    }else{
        bitmap[i] &= ~(0x1 << offset);
    }
}

physaddr_t kalloc_page_frame_at(physaddr_t addr){
    addr=addr&0xFFFFF000; //Align
    if(is_free_page(addr)){
        set_page_reserved(addr, true);
        return addr;
    }else{
        // Can't allocate frame
    }
    return 0;
}

//TODO: Check this code
physaddr_t kalloc_page_frame(void){
    int i, j;
    for(i = 0; i < BITMAP_SIZE; i++){
        // ARE WE FREE
        if(bitmap[i] != 0xFFFFFFFF){
            for(j = 0; j < 32; j++){
                // Find the bit to set
                if(bitmap[i] & (1<< j)){
                    bitmap[i] &= ~(1 << j);
                    return (physaddr_t)((i *32 + j) * PAGE_SIZE);
                }
            }
        }  
    }
    return (physaddr_t)(0x0);
}

void kfree_page_frame(physaddr_t addr){
    addr=addr&0xFFFFF000;
    set_page_reserved(addr, false);
}
