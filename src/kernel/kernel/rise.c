#include <stdio.h>
#include <stdlib.h>

#include <kernel/pmm.h>
#include <kernel/spinlock.h>
#include <kernel/vmm.h>

//uint32_t curraddr = heap_start;
spinlock lock = 0;

static int page_size = -1;


int liballoc_lock()
{
	spin_lock(&lock);
	return 0;
}

int liballoc_unlock()
{
	spin_unlock(&lock);
	return 0;
}

void* liballoc_alloc( int pages )
{
	unsigned int size = pages * PAGE_SIZE;

    if((int)(size + heap_cur) >= (int)(heap_end)){
        qemu_printf("Out of memory: You need a bigger heap pal\n");
        return 0;
    }else{

        uint32_t end_va = heap_cur + (size);
        allocate_region(kernel_dir, heap_cur, end_va);
	
        void * rez = heap_cur;
        heap_cur += size;

	    return (void*) rez;
    }
}

int liballoc_free( void* ptr, int pages )
{
    uint32_t end_va = ptr + (pages * PAGE_SIZE);
    free_region(kernel_dir, (uint32_t) ptr, end_va);
	return 1; // TODO: Fix this so it returns 1 if it actually works
}
