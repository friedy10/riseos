/*
 * =====================================================================================
 *
 *       Filename:  vmm.c
 *
 *    Description: Virtual memory manager using identit-based paging
 *
 *        Version:  1.0
 *        Created:  07/23/2020 12:34:00 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Friedrich Doku, 
 *   Organization:  Mutex Unlocked
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <kernel/pmm.h>
#include <kernel/vmm.h>
    
uint32_t endkerneladdr;
extern uint32_t page_directory;
static uint32_t* &page_directory;
static page_directory *kernel_pdir=0;


void set_page(vaddr_t addr){
    unsigned pdi = addr >> 22;
    unsigned pti = (addr & 0x003FFFFF) >> 12;
    unsigned pdindex = pdi* 0x400 + pti;
    PAGE_DIRECTORY[pdindex] = flags;
    __asm__ volatile("invlpg [0%]" :: "r"(
                (uint32_t)&PAGE_DIRECTORY[pdindex]) : "memory");
}

uint32_t get_page(vaddr_t addr){
    unsigned pdi = addr >> 22;
    unsigned pti = (addr & 0x003FFFFF) >> 12;
    unsigned pdindex = pdi*0x400 + pti;
    return PAGE_DIRECTORY[pdindex];
}

uint32_t virt_to_phys(vaddr_t addr){
    return addr & 0xfffff000;
}

uint32_t get_page_table_entry(uint32_t pdi, uint32_t pti){
    unsigned pdindex = pdi*0x400 + pti;
    return PAGE_DIRECTORY[pdindex];
}

void set_page_table_entry(uint32_t pdi, uint32_t pti, uint32_t flags){
    unsigned pdindex = pdi*0x400 + pti;
    PAGE_DIRECTORY[pdindex] = flags;
    __asm__ volatile("invlpg [0%]" :: "r"(
                (uint32_t)&PAGE_DIRECTORY[pdindex]) : "memory");
}

bool page_table_exist(uint32_t pdi, uint32_t pti){
    return get_page_table_entry(pdi, pti) & PRESENT;
}

// Setup a page table
void new_page_table(unsigned pdi, bool kernel, bool readwrite){
    // Load a page for the page table
    physaddr_t paddr = kalloc_page_frame();
    uint32_t flags = PRESENT;
    if(!kernel) flags |= USERMODE;
    if(readwrite) flags |= READWRITE;
    set_page_table_entry(1023, pdi, paddr | flags);
    for(unsigned i = 0; i < 1024; i++){
        set_page_table_entry(pdi, i, 0);
    }
}
