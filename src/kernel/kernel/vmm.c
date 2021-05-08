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
#include <stdlib.h>
#include <stdio.h>
#include <kernel/pmm.h>
#include <kernel/vmm.h>
#include <kernel/serial.h>


void* dumballoc(uint32_t size){
    void * ret = (void*) kalloc_page_frame();
    size -= PAGE_SIZE;

    for(int i = 0; i < size; i+=PAGE_SIZE){
       kalloc_page_frame(); 
    }

    return ret;
}

void paging_init(){
   
    page_directory_t* page_directory =
        (page_directory_t*) (dumballoc(sizeof(page_directory_t)));
    qemu_printf("Address of page directory 0x%p\n", (void*) page_directory);
    memset(page_directory, 0, sizeof(page_directory_t));

    for(int i = 0; i < 1024; i++){
       /// page_directory->table[i] =  0x00000002;
    }
    

    page_table_t* page_table = 
        (page_table_t*) (dumballoc(sizeof(page_table_t)));
    qemu_printf("Address of page table 0x%p\n", (void*) page_table);
    memset(page_table, 0, sizeof(page_table_t));

    for(unsigned int i = 0; i < 1024; i++){
        page_table->page[i] = ((uint32_t) kalloc_page_frame()) | 3;
    }

    /*  for(unsigned int i = 0; i < 1024; i++){
        page_table->page[i].frame = kalloc_page_frame() >> 12;
        page_table->page[i].rw = 1;
        page_table->page[i].present = 1;
    }
    
    if(!page_directory){
        qemu_printf("Page directory is empty\n");
        return;
    }*/

   /*   page_directory->table[0].present = 1;
    page_directory->table[0].rw = 1;
    page_directory->table[0].user = 1;
    page_directory->table[0].frame = ((unsigned int) page_table);
    page_directory->ref_table[0] = page_table;*/

    page_directory->table[0] = (page_table_t) ((unsigned int) page_table) |3;

    qemu_printf("Running paging_init\n");
    
    qemu_printf("Address of page directory 0x%p\n", (void*) page_directory);
    load_page_directory(page_directory);
    qemu_printf("Loaded page directory\n");

    enable_paging();
    qemu_printf("Paging enabled\n");
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
