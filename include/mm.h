#ifndef _MM_H
#define _MM_H

#include <defs.h>
#include <sched.h>
#include <sys/elf.h>


struct vm_area_struct {
        struct mm_struct             *vm_mm;        /* associated mm_struct */
        unsigned long                vm_start;      /* VMA start, inclusive */
        unsigned long                vm_end;        /* VMA end , exclusive */
        struct vm_area_struct        *vm_next;      /* list of VMA's */
//        pgprot_t                     vm_page_prot;  /* access permissions */
        unsigned long                vm_flags;      /* flags */
//        struct rb_node               vm_rb;         /* VMA's node in the tree */
//        struct list_head             anon_vma_node;     /* anon_vma entry */
//        struct anon_vma              *anon_vma;         /* anonymous VMA object */
        struct vm_operations_struct  *vm_ops;           /* associated ops */
        unsigned long                vm_pgoff;          /* offset within file */
//        struct file                  *vm_file;          /* mapped file, if any */
//        void                         *vm_private_data;  /* private data */
};

//void (*open) (struct vm_area_struct *);
//void (*close) (struct vm_area_struct *);
//struct page * (*nopage) (struct vm_area_struct *, unsigned long, int);

struct vm_area_struct* populate_vma(elf64_eheader *elf_ehdr, elf64_pheader *elf_phdr);
struct vm_area_struct * find_vma(struct mm_struct *mm, unsigned long addr);



//unsigned long do_mmap(struct file *file, unsigned long addr,
//                      unsigned long len, unsigned long prot,
//                      unsigned long flag, unsigned long offset);


//int do_munmap(struct mm_struct *mm, unsigned long start, size_t len);

//Also write the system calls for both the do_mmap and do_munmap functions.

void* malloc_pcb(uint64_t requestsize,void **physfree,uint64_t *pml4);

#endif