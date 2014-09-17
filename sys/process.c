#include <stdio.h>
#include <sched.h>
#include <pmap.h>
#include <sys/tarfs.h>

struct task_struct *task1, *task2, *task3;

struct task_struct *current_task;

uint64_t rip1 = 0;

void first_switch()
{
	__asm__(
		"movq %0, %%rsp;"
		:
		:"r"((current_task->rsp))
	);

	asm volatile(
		"pop %r15;"
		"pop %r14;"
		"pop %r13;"
		"pop %r12;"
		"pop %r11;"
		"pop %r10;"
		"pop %r9;"
		"pop %r8;"
		"pop %rdi;"
		"pop %rsi;"
		"pop %rdx;"
		"pop %rcx;"
		"pop %rbx;"
		"pop %rax;"
	);

	__asm__(
		"iretq"
	);
}

void schedule()
{
	asm volatile(
		"movq (%%rsp), %0"
		:"=r"(rip1)
	);

	asm volatile(
		"popq %rax;"
	);

	asm volatile(
		"movq %%rsp, %0"
		:"=m"(current_task->rsp)
		:
		:"memory"
	);	

	struct task_struct * temp = current_task;
	while(temp->next_task != NULL)
	{
		temp = temp->next_task;
	}
	temp->next_task = current_task;
	temp = temp->next_task;
	current_task = current_task->next_task;
	temp->next_task = NULL;

	asm volatile(
		"movq %0, %%rsp "
		:
		:"m"(current_task->rsp)
		:"memory"
	);

	asm volatile("jmp %0"::"r"(rip1));
}

void func1()
{
	while(1)
	{
		printf("\nHello");
		__asm__("int $0x80");
	}
}

void func2()
{
	while(1)
	{
		printf("\tWorld");
		__asm__("int $0x80");
	}
}

void func3()
{
	while(1)
	{
		printf("\tBye");
		__asm__("int $0x80");
	}
}

void init_process(struct task_struct * proc, void * func)
{
	proc->rip = (uint64_t)func;

	proc->stack[63] = 0x10;
	proc->stack[62] = (uint64_t)&proc->stack[63];
	proc->stack[61] = 0x20202;
	proc->stack[60] = 0x08;
	proc->stack[59] = (uint64_t)func;

	proc->rsp = (uint64_t)&proc->stack[45];
}

void add_proc_to_task_list(void * func)
{
	struct task_struct * task = kmalloc(sizeof(struct task_struct));
	init_process(task, func);

	if (current_task == NULL)
	{
		current_task = task;
		current_task->next_task = NULL;
	}
	else
	{
		struct task_struct * temp = current_task;
		while(temp->next_task != NULL)
		{
			temp = temp->next_task;
		}
		temp->next_task = task;
		task->next_task = NULL;
	}
}




void add_userproc_to_task_list()
{
	struct task_struct * task = load_elf("bin/hello"); //kmalloc(sizeof(struct task_struct));
	CopyCodeData(task);
	//init_process(task, func);

	if (current_task == NULL)
	{
		current_task = task;
		current_task->next_task = NULL;
	}
	else
	{
		struct task_struct * temp = current_task;
		while(temp->next_task != NULL)
		{
			temp = temp->next_task;
		}
		temp->next_task = task;
		task->next_task = NULL;
	}
}

void setup_processes()
{	
	add_proc_to_task_list(func1);
	//add_proc_to_task_list(func2);
	//add_proc_to_task_list(func3);
	add_userproc_to_task_list();



	first_switch();
}
