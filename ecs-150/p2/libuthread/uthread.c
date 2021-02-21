#include <assert.h>
#include <limits.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"
#include "queue.h"

// Get list of thread states
enum thread_states
{
        READY,
        RUNNING,
        BLOCKED,
        ZOMBIE
};

// Establish thread control block
typedef struct TCB
{
        uthread_t tid;
        enum thread_states state;
        void *stack;
        uthread_ctx_t *ctx;
        int retval; // useful in exit()
        uthread_t ptid; // useful to find parent thread to unblock
        int joined; // useful to check if a thread is already joined
} TCB;

// Use `static` on private data structure 
static TCB *running_thread;
int tid_count = 0;

static queue_t ready;
static queue_t zombie;
static queue_t blocked;

// ------------------------------------------------------ //
//                     HELPER FUNCTION                    //
// ------------------------------------------------------ //

// find_tid() - find the corresponding thread on TID
// Source: project_2.html (`find_item` function)
int find_tid(queue_t queue, void *data, void *arg)
{
        TCB *thread = data;
        uthread_t match = *(uthread_t*) arg; // avoid cast error
        (void) queue; // unused

        if (thread->tid == match)
        {
                return 1;
        }

        return 0;
}

// ------------------------------------------------------ //
//                     THREAD FUNCTION                    //
// ------------------------------------------------------ //

// uthread_start() - start multithreading library
// Initialize main thread & create queues
// NOTE: No context creation is needed (Discussion 2/5)
int uthread_start(int preempt)
{
        running_thread = malloc(sizeof(TCB));
        if (!running_thread)
        {
                return -1;
        }

        // Initialize information (except context)
        running_thread->tid = 0;
        running_thread->state = RUNNING;
        running_thread->stack = uthread_ctx_alloc_stack();
        running_thread->ctx = malloc(sizeof(uthread_ctx_t));

        // Create queues
        ready = queue_create();
        zombie = queue_create();
        blocked = queue_create();

        // Enable preemption if preempt is true
        if (preempt != 0)
        {
                preempt_start();
        }

        return 0;
}

// uthread_stop() - Stop the multithreading library
// Destroy all empty queues and reset tid cout to 0
// Return -1 if any destruction fails
int uthread_stop(void)
{
        // Stop preemption
        preempt_stop();

        // Destroy empty queues
        if (queue_destroy(ready) != 0)
        {
                return -1;
        }

        if (queue_destroy(zombie) != 0)
        {
                return -1;
        }

        if (queue_destroy(blocked) != 0)
        {
                return -1;
        }

	tid_count = 0; // Source: Piazza @440
        return 0;
}

// uthread_create() - create a new thread
// NOTE: Do need context creation
int uthread_create(uthread_func_t func)
{
        // If TID overflow or null func, return -1
        // Source: Piazza @451 & @454
        if (tid_count >= USHRT_MAX + 1 || func == NULL)
        {
                preempt_enable();
                return -1;
        }

        // Allocate new thread
        TCB *new_thread = malloc(sizeof(TCB));
        if (!new_thread)
        {
                preempt_enable();
                return -1;
        }

        // Enter critical section
        preempt_disable();

        // Initialize information for new thread
        tid_count++;
        new_thread->tid = tid_count;
        new_thread->state = READY;
        new_thread->stack = uthread_ctx_alloc_stack();
        new_thread->ctx = malloc(sizeof(uthread_ctx_t));
        if (uthread_ctx_init(new_thread->ctx, new_thread->stack, func) == -1)
        {
                preempt_enable();
                return -1;
        }

        // Put on ready queue
        queue_enqueue(ready, new_thread);

        // Exit critical section
        preempt_enable();
        return 0;
}

// uthread_yield() - Yield execution to first thread on ready
// Lecture 06.process-scheduling.pdf (page 9/19)
// NOTE: only between READY & RUNNING
void uthread_yield(void)
{
        TCB *from_thread = running_thread;
        TCB *to_thread;

        // Enter CS
        preempt_disable();

        // Dequeue first thread on ready to run
        queue_dequeue(ready, (void**)&to_thread);
        to_thread->state = RUNNING;

        // Put current thread on ready
        from_thread->state = READY;
        queue_enqueue(ready, from_thread);

        // Context switch
        running_thread = to_thread;
        uthread_ctx_switch(from_thread->ctx, to_thread->ctx);

        // Exit CS
        preempt_enable();
}

// uthread_self() - Return ID of current thread
uthread_t uthread_self(void)
{
	return running_thread->tid;
}

// uthread_exit() - Finish execution of a thread
// Three cases: running to zombie, ready to running
// and blocked to ready
// NOTE: Do unblocking first so ready can have something
// Source: Piazza @347
void uthread_exit(int retval)
{
        // Enter CS
        preempt_disable();

        // Put finished thread to zombie
        TCB *running_to_zombie = running_thread;
        running_to_zombie->retval = retval;
        running_to_zombie->state = ZOMBIE;
        queue_enqueue(zombie, running_to_zombie);

        // Get corresponding blocked thread to ready queue (unblocking)
        if (queue_length(blocked) > 0)
        {
                TCB *blocked_to_ready = malloc(sizeof(TCB));
                queue_iterate(blocked, find_tid, &running_to_zombie->ptid,
                        (void**)&blocked_to_ready);
        
                if (blocked_to_ready != NULL)
                {
                        TCB *saved_blocked = blocked_to_ready;
                        queue_delete(blocked, saved_blocked);

                        blocked_to_ready->state = READY;
                        queue_enqueue(ready, blocked_to_ready);
                }
        }

        // Yield execution to the first thread on ready
        if (queue_length(ready) > 0)
        {
                TCB *ready_to_running = malloc(sizeof(TCB));
                queue_dequeue(ready, (void**)&ready_to_running);
                ready_to_running->state = RUNNING;
        
                running_thread = ready_to_running;
                uthread_ctx_switch(running_to_zombie->ctx, ready_to_running->ctx);
        }

        // Exit CS
        preempt_enable();
}

// uthread_join() - Parent joins child thread
// Get the child thread based on TID in ready and zombie
// If in ready, block parent and yield execution to child
// If in zombie, collect and free resources of child
int uthread_join(uthread_t tid, int *retval)
{
        TCB *parent = running_thread;
        TCB *active_child;
        TCB *dead_child;

        // Enter CS
        preempt_disable();

        // Loop through ready and zombie queues to find TID
	queue_iterate(ready, find_tid, &tid, (void**)&active_child);
        queue_iterate(zombie, find_tid, &tid, (void**)&dead_child);

        // If no tid found, return -1
        if (active_child == NULL && dead_child == NULL)
        {
                preempt_enable();
                return -1;
        }

        // If tid in ready, block parent and excute child
        else if (active_child != NULL)
        {
                // If already joined, return -1
                if (active_child->joined == 1)
                {
                        preempt_enable();
                        return -1;
                }

                // Else, block parent and yield execution
                // to child
                parent->state = BLOCKED;
                queue_enqueue(blocked, parent);

                queue_dequeue(ready, (void**)&active_child);
                active_child->state = RUNNING;
                active_child->ptid = parent->tid;
                active_child->joined = 1;

                running_thread = active_child;
                uthread_ctx_switch(parent->ctx, active_child->ctx);
        }

        // If in zombie, collect retval and free resources
        else if (dead_child != NULL)
        {
                if (retval != NULL)
                {
                        *retval = dead_child->retval;
                }

                queue_delete(zombie, dead_child);
                uthread_ctx_destroy_stack(dead_child->stack);
                free(dead_child);
        }

        // Exit CS
        preempt_enable();
        return 0;
}