/*
 * =====================================================================================
 *
 *       Filename:  vmm.h
 *
 *    Description: Header file for virutal memory mangare 
 *
 *        Version:  1.0
 *        Created:  07/25/2020 02:27:34 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Friedrich Doku, 
 *   Organization:  Mutex Unlocked
 *
 * =====================================================================================
 */

#ifndef VMM_H
#define VMM_H

#include <stdint.h>
#include <stdbool.h>

#define PAGE_SIZE 4096

// Page Alignment stuff
#define IS_ALIGN(addr) ((((uint32_t)(addr)) | 0xFFFFF000) == 0)
#define ALIGN_PAGE(addr) ((((uint32_t)(addr)) & 0xFFFFF000) + 0x1000)

// Find stuff
#define PAGEDIR_INDEX(vaddr) (((uint32_t)(vaddr)) >> 22)
#define PAGETBL_INDEX(vaddr) (((uint32_t)(vaddr)) >> 12) & 0x3FF
#define PAGEFRAME_INDEX(vaddr) (((uint32_t)(vaddr)) & 0xFFF)

// ERROR CODE
#define ERR_PRESENT  0x1
#define ERR_RW       0x2
#define ERR_USER     0x4
#define ERR_RESERVED 0x8
#define ERR_INST     0x10

typedef struct page_table_entry {
    unsigned int present    : 1;
    unsigned int rw         : 1;
    unsigned int user       : 1;
    unsigned int reserved   : 2;
    unsigned int accessed   : 1;
    unsigned int dirty      : 1;
    unsigned int reserved2  : 2;
    unsigned int available  : 3;
    unsigned int frame      : 20;
}page_table_entry_t;

typedef struct page_dir_entry {
    unsigned int present    : 1;
    unsigned int rw         : 1;
    unsigned int user       : 1;
    unsigned int w_through  : 1;
    unsigned int cache      : 1;
    unsigned int access     : 1;
    unsigned int reserved   : 1;
    unsigned int page_size  : 1;
    unsigned int global     : 1;
    unsigned int available  : 3;
    unsigned int frame      : 20;
}page_dir_entry_t;

typedef struct page_table{
    uint32_t page[1024];
} page_table_t;

typedef struct page_directory{
    page_table_t table[1024];
} page_directory_t;

extern void load_page_directory(unsigned int*);
extern void enable_paging();

void* dumb_kmalloc(uint32_t, int);
void paging_init();
void * virtual2phys(void);

#endif
