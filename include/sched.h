#ifndef _SCHED_H
#define _SCHED_H

#include <defs.h>


#define TASK_RUNNING            0
#define TASK_INTERRUPTIBLE      1
#define TASK_UNINTERRUPTIBLE    2
#define TASK_STOPPED            4
#define TASK_TRACED             8
/* in tsk->exit_state */
#define EXIT_ZOMBIE             16
#define EXIT_DEAD               32
/* in tsk->state again */
#define TASK_NONINTERACTIVE     64

#define STACK_SIZE 4096

struct task_struct;
struct mm_struct;

struct task_struct {
	struct mm_struct *mm;
	uint64_t rip;
	uint64_t rsp;
       uint64_t stack[STACK_SIZE];
       void (*fptr)(void);
	uint64_t pid;
	volatile long state;	/* -1 unrunnable, 0 runnable, >0 stopped */
	long counter;
	long priority;
	struct task_struct *next_task;
	uint64_t cr3;
};


struct mm_struct {
        struct vm_area_struct  *mmap;               /* list of memory areas */
        //struct rb_root         mm_rb;               /* red-black tree of VMAs */
        struct vm_area_struct  *mmap_cache;         /* last used memory area */
        unsigned long          free_area_cache;     /* 1st address space hole */
        //pgd_t                  *pgd;                /* page global directory */
        //atomic_t               mm_users;            /* address space users */
         int                    mm_count;            /* primary usage counter */
         int                    map_count;           /* number of memory areas */
        //struct list_head       mmlist;              /* list of all mm_structs */
        unsigned long          start_code;          /* start address of code */
        unsigned long          end_code;            /* final address of code */
        unsigned long          start_data;          /* start address of data */
        unsigned long          end_data;            /* final address of data */
        unsigned long          rss;                 /* pages allocated */        
	 int pid;
};

void setup_processes();
void schedule();

#define for_each_task(p) \
	for (p = &init_task ; (p = p->next_task) != &init_task ; )

#define next_task(p)    list_entry((p)->tasks.next, struct task_struct, tasks)

#define REMOVE_LINKS(p) do { unsigned long flags; \
	(p)->next_task->prev_task = (p)->prev_task; \
	(p)->prev_task->next_task = (p)->next_task; \
	if ((p)->p_osptr) \
		(p)->p_osptr->p_ysptr = (p)->p_ysptr; \
	if ((p)->p_ysptr) \
		(p)->p_ysptr->p_osptr = (p)->p_osptr; \
	else \
		(p)->p_pptr->p_cptr = (p)->p_osptr; \
	} while (0)

#define SET_LINKS(p) do { unsigned long flags; \
	(p)->next_task = &init_task; \
	(p)->prev_task = init_task.prev_task; \
	init_task.prev_task->next_task = (p); \
	init_task.prev_task = (p); \
	(p)->p_ysptr = NULL; \
	if (((p)->p_osptr = (p)->p_pptr->p_cptr) != NULL) \
		(p)->p_osptr->p_ysptr = p; \
	(p)->p_pptr->p_cptr = p; \
	} while (0)



#endif