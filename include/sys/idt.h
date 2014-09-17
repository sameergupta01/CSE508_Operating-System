#include <defs.h>

void reload_idt();
struct idt_entry_struct
{
    uint16_t offset_low;
    uint16_t selector;
    unsigned ist : 3;
    unsigned reserved0 : 5;
    unsigned type : 4;
    unsigned zero : 1;
    unsigned dpl : 2;
    unsigned p : 1;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t reserved1;
} __attribute__((packed));

typedef struct idt_entry_struct idt_entry_t;

struct idt_ptr_struct
{
    uint16_t limit;
    uint64_t base;
} __attribute__ ((packed));

typedef struct idt_ptr_struct idt_ptr_t;

struct regs
{
    unsigned int gs, fs, es, ds; /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax; /* pushed by 'pusha' */
    unsigned int int_no, err_code; /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss; /* pushed by the processor automatically */
};

void idt_set_gate(int num,uint16_t selector, unsigned ist, unsigned type, unsigned dpl, uint64_t offset);

void init_isr();
void init_irq();
