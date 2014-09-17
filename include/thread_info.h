#include <defs.h>
#include <sched.h>

/*
  * We use bit 30 of the preempt_count to indicate that kernel
  * preemption is occurring.  See <asm/hardirq.h>.
*/
#define PREEMPT_ACTIVE  0x40000000
#define INIT_PREEMPT_COUNT      (1 + PREEMPT_ACTIVE)
#define KERNEL_DS       0x00000000
#define get_ds()        (KERNEL_DS)

struct thread_info {
           struct task_struct      *task;          /* main task structure */
           //struct exec_domain      *exec_domain;   /* execution domain */
           unsigned long           flags;          /* low level flags */
           unsigned long           status;         /* thread-synchronous flags */
           __u32                   cpu;            /* current CPU */
           int                     preempt_count;  /* 0 => preemptable, <0 => BUG */
   
   
           mm_segment_t            addr_limit;     /* thread address space:
                                                      0-0xBFFFFFFF for user-thread
                                                      0-0xFFFFFFFF for kernel-thread
                                                   */
           void                    *sysenter_return;
//           struct restart_block    restart_block;
   
           unsigned long           previous_esp;   /* ESP of the previous stack in case
                                                      of nested (IRQ) stacks
                                                   */
           __u8                    supervisor_stack[0];
   };


#define INIT_THREAD_INFO(tsk)                   \
  {                                               \
          .task           = &tsk,                 \
          //.exec_domain    = &default_exec_domain, \
          .flags          = 0,                    \
          .cpu            = 0,                    \
          .preempt_count  = INIT_PREEMPT_COUNT,   \
          .addr_limit     = KERNEL_DS,            \
         // .restart_block = {                      \
         //         .fn = do_no_restart_syscall,    \
          },                                      \
  }