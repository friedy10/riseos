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
#ifndef PPM_H
#define PPM_H

#include <stdint.h>
#include <stdbool.h>
// Used to mark the end of the kernel
extern uint32_t endkernel;

#define BITMAP_SIZE 32768
// 4096
#define PAGE_SIZE 0x1000
#define WORDSIZE 32
#define BIT_WS 17
#define BITMASK 0x1f
typedef uintptr_t physaddr_t;

uint32_t bitmap[BITMAP_SIZE];

void setup_bitmap(void);
bool is_free_page(physaddr_t p);
void kalloc_page_frame();
physaddr_t kfree_page_frame(physaddr_t ptr);
physaddr_t kalloc_page_frame_at(physaddr_t addr);

#endif
