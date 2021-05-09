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

//uint32_t endkerneladdr;


uint32_t paddr = (uint32_t)(&endkerneladdr);


void* dumballoc(uint32_t sz)
{
	uint32_t tmp;
	if (paddr& 0xFFF) {
		paddr&= ~0xFFF;
		paddr += 0x1000;
	}
	tmp = paddr;
	paddr += sz;
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

/*void* dumballoc(uint32_t size){
    void * ret = (void*) kalloc_page_frame();
    size -= PAGE_SIZE;

    for(int i = 0; i < size; i+=PAGE_SIZE){
       kalloc_page_frame();
    }

    return ret;
}*/

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
    for(i = 0; i < endkerneladdr; i++){
        table->pages[i].present = 1;
        table->pages[i].writeable = 1;
        table->pages[i].user = 0;
        table->pages[i].frame = i;
    }

    kernel_dir->tables[0] = table;
    kernel_dir->tables_physical[0] = ((uint32_t) table->pages) | 3;
    
    //load_page_directory(kernel_dir);
    //enable_paging();
    
    switch_to_page_dir(kernel_dir);

    qemu_printf("Paging enabled...\n");
    
}


void * virtual2phys(void){
    
}




/* void paging_init(){
    // uint32_t page_directory[1024] __attribute__((aligned(4096)));

    // For some reason paging only works when the directory is 
    // below the adress 0x127000 IDK why

    uint32_t* page_directory = (uint32_t*) (dumballoc(PAGE_SIZE));
    memset(page_directory, 0, 4096);
    
    //set each entry to not present
    for(int i = 0; i < 1024; i++){
        page_directory[i] = 0x00000002;
    }


    uint32_t* first_page_table = (uint32_t*) (dumballoc(PAGE_SIZE));
    memset(first_page_table, 0, 4096);

    for(unsigned int i = 0; i < 1024; i++){
        first_page_table[i] = (i * 0x1000) | 3;
    }

    page_directory[0] = ((unsigned int)first_page_table) | 3;


    qemu_printf("Running paging_init\n");
    
    load_page_directory(page_directory);
    enable_paging();

    qemu_printf("Paging enabled\n");
}*/
