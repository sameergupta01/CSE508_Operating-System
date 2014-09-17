#ifndef _PMAP_H
#define _PMAP_H

#include <defs.h>

void init_freelist(void* physbase, void* physfree);
void set_kernel_in_freelist(uint64_t start_addr, uint64_t end_addr);
void reserve_pages(uint64_t start_addr, uint64_t end_addr);
void* kmalloc_page();
void* kmalloc(uint64_t size_mem);
void* kmalloc_mem();
void* kmalloc_virtual(uint64_t size_mem,uint64_t virt);
void remap_bitmap();

#endif
