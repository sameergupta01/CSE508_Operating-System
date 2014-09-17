#include <defs.h>
#include <stdio.h>
#include <pmap.h>
#include <mm.h>
#include <pagetab.h>
#include <sys/elf.h>
#include <sched.h>

extern char kernmem;


//void open(struct vm_area_struct *area);
//void close(struct vm_area_struct *area);

struct vm_area_struct *find_vma(struct mm_struct *mm, unsigned long addr)
{
    if (mm != NULL) {
        struct vm_area_struct * curr;
        curr = mm->mmap;

        while (curr != NULL) {
	        if (curr && curr->vm_end > addr && curr->vm_start <= addr)
                    return curr;
               curr = curr->vm_next;
		
        }
    }
    return NULL;
}


struct vm_area_struct* populate_vma(elf64_eheader *elf_ehdr, elf64_pheader *elf_phdr)
{
  struct vm_area_struct *vma = (struct vm_area_struct *)kmalloc(sizeof(struct vm_area_struct)); 

    vma->vm_start = elf_phdr->p_vaddr;
    vma->vm_end = elf_phdr->p_vaddr + elf_phdr->p_memsz;
    vma->vm_flags = elf_phdr->p_flags;
    vma->vm_pgoff = elf_phdr->p_offset;
    //vma->grows_down = 0;                   
     //vma->vm_file = (struct file *)kmalloc(sizeof(struct file));
        //vma->vm_file->file_start = (uint64_t)elf_ehdr;
        //vma->vm_file->vm_pgoff = elf_phdr->p_offset;
        //vma->vm_file->vm_sz = elf_phdr->p_filesz;
     vma->vm_next = NULL;
    return vma;

};