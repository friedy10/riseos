/*
 * =====================================================================================
 *
 *       Filename:  vmm.c
 *
 *       Description: Paging Implementation
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

#define NULL 0

#include <stdlib.h>
#include <stdio.h>
#include <kernel/pmm.h>
#include <kernel/vmm.h>
#include <kernel/serial.h>

uint32_t nframes = 0;
uint32_t top = 0;
uint32_t *frames;
page_directory *current_directory = NULL;



void* dumballoc(uint32_t sz)
{
	uint32_t tmp;
	if (placementaddr& 0xFFF) {
		placementaddr&= ~0xFFF;
		placementaddr += 0x1000;
	}
	tmp = placementaddr;
	placementaddr += sz;
	return (void *) tmp;
}

void switch_to_page_dir(struct page_directory *dir)
{
	uint32_t cr0 = 0;
	current_directory = dir;
	asm volatile("mov %0, %%cr3":: "r"(dir->tables_physical));
	asm volatile("mov %%cr0, %0": "=r"(cr0));
	cr0 |= 0x80000000;
	asm volatile("mov %0, %%cr0":: "r"(cr0));
}

// Convert a virtual address to a physical address
void* virt_phys(page_directory* dir, void* vaddr){
    uint32_t pdindex = PAGEDIR_INDEX(vaddr);
    uint32_t ptindex = PAGETBL_INDEX(vaddr);
    uint32_t page_frame_offset = PAGEFRAME_INDEX(vaddr);

    // The page table does not exist
    if(!dir->tables[pdindex]){
        qemu_printf("virt_phys: The page directory dir entry does not exist\n");
        return NULL;
    }

    page_table * table = dir->tables[pdindex];

    if(!table->pages[ptindex].present){
        qemu_printf("virt_phys: The page table entry does not exist\n");
        return NULL;
    }

    uint32_t t = table->pages[ptindex].frame;

    return (void*)  ((t >> 12) + page_frame_offset);
}


// Allocate a page
void allocatepage(page_directory* dir, void* vaddr){
    uint32_t pdindex = PAGEDIR_INDEX(vaddr);
    uint32_t ptindex = PAGETBL_INDEX(vaddr);
    uint32_t page_frame_offset = PAGEFRAME_INDEX(vaddr);

    // The page table does not exist
    if(!dir->tables[pdindex]){
        // Create a new page table
        page_table *table = dumballoc(sizeof(*table));
        memset(table, 0, 1024 *sizeof(*table));
        
        //uint32_t t = virt_phys(dir, table);
        dir->tables_physical[pdindex].frame = (uint32_t)table >> 12;
        dir->tables_physical[pdindex].present = 1;
        dir->tables_physical[pdindex].rw = 1;
        dir->tables_physical[pdindex].user = 1;
        dir->tables_physical[pdindex].page_size = 0;

        dir->tables[pdindex] = table;


    }

    page_table * table = dir->tables[pdindex];

    // The page table entry doesn't exist
    if(!table->pages[ptindex].present){
        // Allocate a page frame
        uint32_t frame = kalloc_page_frame();

        table->pages[ptindex].frame = frame;
        table->pages[ptindex].present = 1;
        table->pages[ptindex].writeable = 1;
        table->pages[ptindex].user = 1;
    }
}


void paging_init(){

    qemu_printf("Paging INIT");
    page_directory *kernel_dir = NULL;
    page_table *table = NULL;

    kernel_dir = dumballoc(sizeof(*kernel_dir));
    memset(kernel_dir, 0, 1024 * sizeof(*kernel_dir));

    table = dumballoc(sizeof(*table));
    memset(table, 0, 1024 * sizeof(*table));

    nframes = 0x100000 / 0x1000; // 16 MB
    top = nframes - 1;

    frames = dumballoc(nframes * sizeof(*frames));

    uint32_t i = 0;
    for(i = 0; i < placementaddr; i++){
        table->pages[i].present = 1;
        table->pages[i].writeable = 1;
        table->pages[i].user = 0;
        table->pages[i].frame = i;
    }


    kernel_dir->tables_physical[0].frame = (uint32_t)table >> 12;
    kernel_dir->tables_physical[0].present = 1;
    kernel_dir->tables_physical[0].rw = 1;
    kernel_dir->tables_physical[0].user = 1;
    kernel_dir->tables_physical[0].page_size = 0;
    kernel_dir->tables[0] = table;

    
    switch_to_page_dir(kernel_dir);

    qemu_printf("Paging enabled...\n");
    
}




