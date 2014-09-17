#include <stdio.h>
#include <sched.h>
#include <linux/queue.h>

void init_queue(queue *q)
{
          q->first = 0;
          q->last = SIZE-1;
          q->count = 0;
}

void push(queue *q, struct task_struct *x)
{
          if (q->count >= SIZE)
            printf("Warning: queue overflow enqueue x=%d\n",x);
          else {
            q->last = (q->last+1) % SIZE;
            q->q[ q->last ] = x;    
            q->count = q->count + 1;
             }
}

struct task_struct * pop(queue *q)
{
      struct task_struct *x=NULL;
      if (q->count <= 0) printf("Warning: empty queue dequeue.\n");               else {
            x = q->q[ q->first ];
            q->first = (q->first+1) % SIZE;
            q->count = q->count - 1;
           }
      return(x);
}

struct task_struct * front(queue *q)
{
      struct task_struct *x=NULL;
      if (q->count <= 0) printf("Warning: empty queue dequeue.\n");               else {
            x = q->q[ q->first ];
           }
      return(x);
}



int Isempty(queue *q)
{
          if (q->count <= 0) return (1);
                  else return (0);
}


void printqueue(queue *q)
{
int i = q->first;
for(;i<=q->last;i++){
struct task_struct *t = NULL;
t = q->q[i];
printf("Element at %d is %x\n",i, &t);
}
}

