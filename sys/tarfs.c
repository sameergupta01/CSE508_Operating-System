#include <defs.h>
#include <stdio.h>
#include <math.h>
#include <sys/tarfs.h>
#include <sys/elf.h>
#include <pmap.h>
#include <mm.h>
#include <sched.h>

#define PAGE_SIZE 4096

void* readTarfile(char *file_name) {

    struct posix_header_ustar * header = (struct posix_header_ustar*)(&_binary_tarfs_start);
    int size_of_file = 0; 
    
    do {
		/*convert header->size string to octal to integer*/
		size_of_file = octal_decimal(atoi(header->size));
		if(strcmp(file_name, header->name) == 0)
		{
			printf("File Name: %s..File Size: %d bytes..Contents of file: ", header->name, size_of_file);
			return (void *)header;
		}

		if(size_of_file > 0) 
			header = header + 1 + (size_of_file / 513 + 1);
       	else
       		header = header + 1;
	} while(header < (struct posix_header_ustar*)&_binary_tarfs_end);

	return NULL;    
}

void populateTask(elf64_eheader *elf_ehdr, elf64_pheader *elf_phdr,struct task_struct *tsk)
{
	if(tsk->mm == NULL)
		return;

	struct vm_area_struct *vma = populate_vma(elf_ehdr, elf_phdr);
    	/*
		PF_X   An executable segment.
		PF_W   A writable segment.
		PF_R   A readable segment.
	*/

	if(elf_phdr->p_flags == (PF_R + PF_X) )	//data
	{
		tsk->mm->start_code = elf_phdr->p_vaddr;
		tsk->mm->end_code = elf_phdr->p_vaddr + elf_phdr->p_memsz;
	}
	else if(elf_phdr->p_flags == (PF_R + PF_W + PF_X) ) //text
	{
		tsk->mm->start_data = elf_phdr->p_vaddr;
		tsk->mm->end_data = elf_phdr->p_vaddr + elf_phdr->p_memsz;
	}

	if(tsk->mm->mmap == NULL)
		tsk->mm->mmap = vma;        
	else {
		struct vm_area_struct *curr = tsk->mm->mmap;
		while(curr->vm_next != NULL)
			curr = curr->vm_next;
		curr->vm_next = vma;       
	}

	vma->vm_mm = tsk->mm;

};

struct task_struct* load_elf(char *filename)
{
	struct elf64_eheader *elf_ehdr;
	struct elf64_pheader *elf_phdr;
	struct task_struct *tsk;

	struct posix_header_ustar *elf_header = (struct posix_header_ustar *)readTarfile(filename);

	if(elf_header == NULL)
		return NULL;

	elf_ehdr = (struct elf64_eheader *)(elf_header+1);
	elf_phdr = (struct elf64_pheader *)((uint64_t)elf_ehdr + elf_ehdr->e_phoff);

	tsk = kmalloc(sizeof(struct task_struct));
	tsk->mm = kmalloc(sizeof(struct mm_struct));

	tsk->rip = elf_ehdr->e_entry;   //entry point address of .text section
	//tsk->rsp =  (uint64_t)kmalloc(PAGE_SIZE) ;	 //allocate a blank page in kernel for stack

	tsk->stack[63] = 0x10;
	tsk->stack[62] = (uint64_t)&tsk->stack[63];
	tsk->stack[61] = 0x20202;
	tsk->stack[60] = 0x08;
	tsk->stack[59] = elf_ehdr->e_entry;

	tsk->rsp = (uint64_t)&tsk->stack[45];


	
	tsk->mm->mmap = NULL;            

	elf_phdr = (struct elf64_pheader *)((uint64_t)elf_ehdr + elf_ehdr->e_phoff);
	int i=0;

	for (i = 0; i < elf_ehdr->e_phnum; ++i) {
		if(elf_phdr->p_type == PT_LOAD) {
			printf("elf header type: PT_LOAD\n");
			populateTask(elf_ehdr, elf_phdr, tsk);
		}
		elf_phdr++;
	}

    
	struct vm_area_struct *vma_stack = (struct  vm_area_struct *)kmalloc(sizeof(struct vm_area_struct));   //for stack allocation
	vma_stack->vm_mm = tsk->mm;
	vma_stack->vm_start = tsk->rsp - PAGE_SIZE;
	vma_stack->vm_end = tsk->rsp;
	vma_stack->vm_flags = (PF_R + PF_W);
	//vma->grows_down = 1;                         // set grows down
	//vma_stack->vm_file = NULL;
	vma_stack->vm_next = NULL;

	if(tsk->mm->mmap == NULL)   
		tsk->mm->mmap = vma_stack;        
	else {
		struct vm_area_struct *curr = tsk->mm->mmap;
		while(curr->vm_next != NULL)
			curr = curr->vm_next;
		curr->vm_next = vma_stack;       
	}

	vma_stack->vm_mm = tsk->mm;
	return tsk;
}



int CopyCodeData(struct task_struct* tsk)
{
	int success = 0;
	struct vm_area_struct* curVma = tsk->mm->mmap;

		while(curVma != NULL)
		{
//			if(curVma->vm_start <= virtAddr && virtAddr <= curVma->vm_end)
//			{
				//based on VMA size we need to change the number of pages beign allocated below
//				AssignPhysicalToVirtual(curVma->vm_start,1);
				kmalloc_virtual(curVma->vm_end-curVma->vm_start,curVma->vm_start);


				int totSize = curVma->vm_end - curVma->vm_start;
				char* phyStart = (char*)(tsk->rip); // + curVma->sectionOffset);

//				char* phyStart = (char*)(tsk->mm->fileStartPointer + curVma->sectionOffset);
				char* addr = (char*)curVma->vm_start;
				while(totSize--)
     				*addr ++ = *phyStart++;

				success = 1;
				break;			
//			}
		}
	return success;
}
