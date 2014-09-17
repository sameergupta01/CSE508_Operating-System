#include <defs.h>
#include <stdio.h>
#include <sys/idt.h>
#include <sched.h>

extern void _isr0();
extern void _isr1();
extern void _isr128();

void init_isr()
{
	idt_set_gate(0, 8, 0, 0x0e, 0, ((uint64_t)&_isr0));
    	//idt_set_gate(1, 8, 0, 0x0e, 0, ((uint64_t)&_isr1)); // we can use this handler later if we want
	idt_set_gate(128, 8, 0, 0x0e, 0, ((uint64_t)&_isr128));
}

char *exception_messages[] =
{
    "Division By Zero"
};

//r->int_no is always 16 for the dvide by zero no matter what u push in the stack
void fault_handler(struct regs *r)
{
    if (r->int_no < 32)
    {
      printf("%s", exception_messages[r->int_no-16]);
      //printf("a constant value in regs %d", r->int_no);     
        for (;;);
    }
}

void interrrupt_schedule()
{
	schedule();
}
