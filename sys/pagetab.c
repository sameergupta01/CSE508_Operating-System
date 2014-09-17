#include <pagetab.h>
#include <pmap.h>
#include <stdio.h>

#define PML4_OFFSET 510

extern char kernmem;

struct PT* getPT(struct PML4 *pml4, uint64_t virt_addr);

uint16_t GetPTIndex(uint64_t lAddress)
{
    return lAddress >> 12 & 0x1FF;
}

uint16_t GetPDIndex(uint64_t lAddress)
{
    return lAddress >> 21 & 0x1FF;
}

uint16_t GetPDPIndex(uint64_t lAddress)
{
    return lAddress >> 30 & 0x1FF;
}

uint16_t GetPML4Index(uint64_t lAddress)
{
    return lAddress >> 39 & 0x1FF;
}

void * setup_page_tables(uint64_t kern_physbase, uint64_t kern_physfree)
{
	// Allocate the base page for all the 4 page tables
	struct PML4 *pml4 = (struct PML4 *)kmalloc_mem();
	memset((void *)pml4, 0, PAGE_SIZE);



	//////////////////// MAP KERNEL //////////////////////

	uint64_t kern_virt_addr = (uint64_t)&kernmem;

	struct PT *pt = getPT(pml4, kern_virt_addr);

	uint64_t kern_virt_base_addr = kern_virt_addr & VIRT_BASE;

	uint64_t kern_physbase_t = kern_physbase;
	uint64_t kern_physfree_t = kern_physfree;

	while(kern_physbase_t <= kern_physfree_t)
	{
		uint64_t kern_virt_addr_t = kern_virt_base_addr | kern_physbase_t;	
		pt -> entries[GetPTIndex(kern_virt_addr_t)] = kern_physbase_t | PR | RW | US;
		kern_physbase_t += PAGE_SIZE;
	}


	//////////////////// MAP VIDEO MEMORY //////////////////////

	uint64_t printf_physbase = 0xB8000;
	uint64_t printf_physfree = 0xB8FA0;
	uint64_t printf_virt_addr = PHY_TO_VIRT(printf_physbase, uint64_t);

	pt = getPT(pml4, printf_virt_addr);

	uint64_t printf_virt_base_addr = printf_virt_addr & VIRT_BASE;

	uint64_t printf_physbase_t = printf_physbase;
	uint64_t printf_physfree_t = printf_physfree;

	while(printf_physbase_t <=  printf_physfree_t)
	{
		uint64_t printf_virt_addr_t = printf_virt_base_addr | printf_physbase_t;
		pt -> entries[GetPTIndex(printf_virt_addr_t)] = printf_physbase_t | PR | RW | US;
		printf_physbase_t += PAGE_SIZE;
	}


	//////////////////// MAP PHYSICAL PAGES //////////////////////

	/*uint64_t total_physbase = 0x0;
	uint64_t total_physfree = 0x00000000070fffff;
	uint64_t total_virt_addr;// total_virt_base_addr, total_virt_addr_t;

	uint64_t total_physbase_t = total_physbase;
	uint64_t total_physfree_t = total_physfree;

	while(total_physbase_t <=  total_physfree_t)
	{
		total_virt_addr = PHY_TO_VIRT(total_physbase_t, uint64_t);
		pt = getPT(pml4, total_virt_addr);
		//total_virt_base_addr = total_virt_addr & VIRT_BASE;
		//total_virt_addr_t = total_virt_base_addr | total_physbase_t;
		pt -> entries[GetPTIndex(total_virt_addr)] = total_physbase_t | PR | RW | US;
		total_physbase_t += PAGE_SIZE;
	}*/


	printf("Before setting CR3\n");
	pml4 -> entries[510] = (uint64_t)pml4 | PR | RW | US;

	printf("PML4-> entries[PML4_OFFSET]: %x\n", pml4 -> entries[510]);

	_cpu_write_cr3((uint64_t)pml4);

	remap_printf();
	remap_bitmap();

	printf("After setting CR3\n\nDONE!!!!\n\n");

	return (struct PML4 *)PHY_TO_VIRT_PML4_BASE(uint64_t);
}

struct PT* getPT(struct PML4 *pml4, uint64_t virt_addr)
{
	uint64_t pml4Index = GetPML4Index(virt_addr);
    	uint64_t pdpIndex = GetPDPIndex(virt_addr);
	uint64_t pdIndex = GetPDIndex(virt_addr);

    	uint64_t pdp = pml4 -> entries[pml4Index] & PAGE_ALIGN;
    	if (!pdp)
    	{
       	struct PDP *newpage = (struct PDP *) kmalloc_mem();
		memset((void *)newpage, 0, PAGE_SIZE);
        	pml4 -> entries[pml4Index] = ((uint64_t)newpage) | PR | RW | US;
		
        	pdp = ((uint64_t)newpage);

    	}

	uint64_t pd = ((struct PDP *)pdp) -> entries[pdpIndex] & PAGE_ALIGN;
    	if (!pd)
    	{
       	struct PD *newpage = (struct PD *) kmalloc_mem();
		memset((void *)newpage, 0, PAGE_SIZE);
        	((struct PDP *)pdp) -> entries[pdpIndex] = ((uint64_t)newpage) | PR | RW | US;

        	pd = ((uint64_t)newpage);

    	}

	uint64_t pt = ((struct PD *)pd) -> entries[pdIndex] & PAGE_ALIGN;
    	if (!pt)
    	{
		struct PT *newpage = (struct PT *) kmalloc_mem();
		memset((void *)newpage, 0, PAGE_SIZE);
       	((struct PD *)pd) -> entries[pdIndex] = ((uint64_t)newpage) | PR | RW | US;

	      	pt = ((uint64_t)newpage);

	}

    	return (struct PT *) pt;
}

void mmap(uint64_t virt_addr,uint64_t phy_addr)
{
	uint64_t pml4Index = GetPML4Index(virt_addr);
    	uint64_t pdp = ((struct PML4 *)PHY_TO_VIRT_PML4_BASE(uint64_t))->entries[pml4Index] & PAGE_ALIGN;
    	if (!pdp)
    	{
       	struct PDP *newpage = (struct PDP *) kmalloc_mem();
//		memset((void *)newpage, 0, PAGE_SIZE);
        	((struct PML4 *)PHY_TO_VIRT_PML4_BASE(uint64_t))->entries[pml4Index] = ((uint64_t)newpage) | PR | RW | US;
		
        	pdp = ((uint64_t)newpage);
		
    	}

    	uint64_t pdpIndex = GetPDPIndex(virt_addr);
	uint64_t pd = ((struct PDP *)PHY_TO_VIRT_PDP_BASE(virt_addr,uint64_t))->entries[pdpIndex] & PAGE_ALIGN;
    	if (!pd)
    	{
       	struct PD *newpage = (struct PD *) kmalloc_mem();
//		memset((void *)newpage, 0, PAGE_SIZE);
        	((struct PDP *)PHY_TO_VIRT_PDP_BASE(virt_addr,uint64_t))->entries[pdpIndex] = ((uint64_t)newpage) | PR | RW | US;

        	pd = ((uint64_t)newpage);
		
    	}

	uint64_t pdIndex = GetPDIndex(virt_addr);
	uint64_t pt =        	((struct PD *)PHY_TO_VIRT_PD_BASE(virt_addr,uint64_t))->entries[pdIndex] & PAGE_ALIGN;
    	if (!pt)
    	{
		struct PT *newpage = (struct PT *) kmalloc_mem();
//		memset((void *)newpage, 0, PAGE_SIZE);
       	((struct PD *)PHY_TO_VIRT_PD_BASE(virt_addr,uint64_t))->entries[pdIndex] = ((uint64_t)newpage) | PR | RW | US;

	      	pt = ((uint64_t)newpage);
		
	}
	((struct PT *) PHY_TO_VIRT_PT_BASE(virt_addr,uint64_t))->entries[GetPTIndex(virt_addr)] = phy_addr | PR | RW | US;


}
