#include <defs.h>
#include <kbc.h>
#include <io.h>
#include <stdio.h>
#include <sys/timer.h>
#include <sys/pic.h>
#include <sys/idt.h>

extern void _irq0();
extern void _irq1();

void init_irq()
{
    idt_set_gate(32, 8, 0, 0x0e, 0, ((uint64_t)&_irq0));	//for timer
    idt_set_gate(33, 8, 0, 0x0e, 0, ((uint64_t)&_irq1));	//for kb

    asm volatile("cli");
    PIC_remap(0x20, 0x29);
    IRQ_clear_mask(0);
    asm volatile("sti");
}

void irq_timer()
{
    outb(0x20, 0x20);
    timer_callback();
}

void irq_kb(struct regs * r)
{
    //printf("a constant value in regs %d", r->int_no);
    //printf("call_kb");
    outb(0x20, 0x20);    
    kbc_irq();      
}
