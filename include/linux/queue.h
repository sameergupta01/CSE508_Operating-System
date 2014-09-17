#ifndef QUEUE
#define QUEUE

#include <sched.h>

#define SIZE       100

typedef struct {
        struct task_struct* q[SIZE+1];    /* body of queue */
        int first;                      /* position of first element */
        int last;                       /* position of last element */
        int count;                      /* number of queue elements */
} queue;

int isempty(queue *q);
void printqueue(queue *q);
struct task_struct * pop(queue *q);
struct task_struct * front(queue *q);
void push(queue *q, struct task_struct *x);
void init_queue(queue *q);

#endif
