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

struct page {
	uint32_t present : 1;
	uint32_t writeable : 1;
	uint32_t user : 1;
	uint32_t rsvd_0 : 2;
	uint32_t accessed : 1;
	uint32_t dirty : 1;
	uint32_t rsvd_1 : 2;
	uint32_t avail : 3;
	uint32_t frame : 20;
} __attribute__((packed));

struct page_table {
	struct page pages[1024];
} __attribute__((packed));

struct page_directory {
	struct page_table *tables[1024];
	uint32_t tables_physical[1024];
	uint32_t physical_addr;
};

typedef struct page page;
typedef struct page_table page_table;
typedef struct page_directory page_directory;

extern void load_page_directory(unsigned int*);
extern void enable_paging();

void* dumb_kmalloc(uint32_t, int);
void paging_init();
void * virtual2phys(void);

#endif
