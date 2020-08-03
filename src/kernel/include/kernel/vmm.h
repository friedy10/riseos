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

#ifndef PMM_H
#define PMM_H

#include <stdint.h>
#include <stdbool.h>

#define PRESENT 0x1
#define READWRITE 0x2
#define USERMODE 0x4

#define PAGE_DIRECTORY ((uint32_t*) 0xFFC00000)

typedef struct page_directory_entry
{
	union
	{
		struct
		{
			unsigned flags : 12;
			unsigned addr : 20;
		} __attribute__((packed));
		uint32_t as_uint32;
	};
} __attribute__((packed)) page_directory_entry;

typedef struct page_directory
{
	page_directory_entry entries[1024];
} page_directory;


#endif
