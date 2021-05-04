/*
 * =====================================================================================
 *
 *       Filename:  pmm.h
 *
 *    Description: Physical memeory manager 
 *
 *        Version:  1.0
 *        Created:  07/20/2020 08:10:59 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Friedrich Doku, 
 *   Organization:  Mutex Unlocked
 *
 * =====================================================================================
 */
#ifndef PMM_H
#define PMM_H

#include <stdint.h>
#include <stdbool.h>


#define BLOCK_SIZE 4096
#define BLOCKS_PER_BUCKET 8
// Define some bit manipulating operations
#define SETBIT(i) bitmap[i / BLOCKS_PER_BUCKET] = bitmap[i / BLOCKS_PER_BUCKET] | (1 << (i % BLOCKS_PER_BUCKET))
#define CLEARBIT(i) bitmap[i / BLOCKS_PER_BUCKET] = bitmap[i / BLOCKS_PER_BUCKET] & (~(1 << (i % BLOCKS_PER_BUCKET)))
#define ISSET(i) ((bitmap[i / BLOCKS_PER_BUCKET] >> (i % BLOCKS_PER_BUCKET)) & 0x1)
#define GET_BUCKET32(i) (*((uint32_t*) &bitmap[i / 32]))

#define BLOCK_ALIGN(addr) (((addr) & 0xFFFFF000) + 0x1000)

// Used to mark the end of the kernel
extern uint32_t endkernel;
extern uint32_t endkerneladdr;

#define BITMAP_SIZE 32768
#define PAGE_SIZE 0x1000
#define WORDSIZE 32
#define BIT_WS 17
#define BITMASK 0x1f
typedef uintptr_t physaddr_t;

void pmm_init(void);
bool is_free_page(physaddr_t p);
physaddr_t kalloc_page_frame(void);
void kfree_page_frame(physaddr_t ptr);
physaddr_t kalloc_page_frame_at(physaddr_t addr);

#endif
