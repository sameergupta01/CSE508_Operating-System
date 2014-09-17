.text

######
# load a new ISR
.global _isr0
_isr0:
    cli
    pushq  $0 # error code
    pushq  $0 # idt no
    jmp isr_common_stub

.global _isr1
_isr1:
    cli
    pushq $0
    pushq $1
    jmp isr_common_stub

.global _isr128
_isr128:
    cli
    jmp isr_interrupt_schedule_stub


.extern isr_handler

isr_common_stub:
    pushq %rax
    pushq %rcx
    pushq %rdx
    pushq %rsi
    pushq %rdi
    pushq %r8
    pushq %r9
    pushq %r10
    pushq %r11
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15
    movq %rsp, %rdi
    addq $72, %rdi
    callq fault_handler
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %r11
    popq %r10
    popq %r9
    popq %r8
    popq %rdi
    popq %rsi
    popq %rdx
    popq %rcx
    popq %rax
    sti
    iretq


isr_interrupt_schedule_stub:
    pushq %rax
    pushq %rbx
    pushq %rcx
    pushq %rdx
    pushq %rsi
    pushq %rdi
    pushq %r8
    pushq %r9
    pushq %r10
    pushq %r11
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15
    callq schedule
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %r11
    popq %r10
    popq %r9
    popq %r8
    popq %rdi
    popq %rsi
    popq %rdx
    popq %rcx
    popq %rbx
    popq %rax
    sti
    iretq
