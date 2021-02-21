# ECS 150 - Project 2 Report

Student names:
- Xuan Huang: xwhuang@ucdavis.edu, 915840915
- Han Nguyen: hnknguyen@ucdavis.edu, 917278789

## Phase 1: Queue.c
In this phase, we look at how the user-library threading works in the bigger  
picture. Hence, we determine that we should use a linked list because it  
provides us dynamic flexibility in interacting with threads, as threads are  
stored in the FIFO order. We do not choose to use doubly-linked-list since  
our user-library threading implemention does not require two-way traversal.

The four functions `queue_create`, `queue_destroy`, `queue_enqueue` and  
`queue_length` simply does what it is supposed to do in a linked list -  
initialize data of node, destroy empty queues, put data on the back of the  
queue and return length of queue. 

In `queue_dequeue` we save data of the first queue in `*data` and save the  
head node before moving head node to the next node. We do this to avoid  
destroying the link if freeing the head node right away.

In `queue_delete`, we have two cases: (1) found at head (then we do similarly  
with `queue_dequeue`), and (2) if not, then we loop through the queue and  
stop at where the data is found. If not found, return -1. If found, we remove  
the node by merging current node with the next node.

In `queue_iterate`, we loop through the queue to store the data where `func`  
returns 1 and stop iteration. To provide feature for deletion resistance, we  
save the address of the next node before calling `func`. We do this to make  
the loop continue even if the data item is deleted.

## Phase 2 & 3: Uthread.c
In this phase, we look at the bigger picture of the user-threading library.

(1) We decide to add three data in the thread control block structure besides  
the required four: `retval`, `ptid` and `joined`. This will help keep track  
of the return value of the dead process, the parent process of a dead process  
and check if the process has been joined before.

(2) We have three queues (`ready`, `blocked`, `zombie`) to store threads.  
Since only one thread is running at the same (concurrency not paralellism)  
in round-robin fasion, we decide not to use a queue for running, but a global  
struct pointer instead to save memory space.

(3) We use `static` in order to protect privacy of the data structure against  
other files (instead of `extern` as default).

(4) We have a helper function `find_tid` (inspired by `find_item` in the  
project prompt) to find a thread with a specific thread ID.

In `uthread_start`, we initalize the global struct of running thread as the  
main thread, with thread ID = 0, state of `RUNNING` and allocate stack. We do  
not initalize context for main thread because we learn that the context for  
main is already saved, stated by professor. We create the three queues and  
start preemption (if required by user). After the multithreading stops, we  
stop preemption, destroy empty queues and reset thread ID to 0 in  
`uthread_stop`.

The next three functions `uthread_create`, `uthread_yield` and `uthread_self`  
primarily create and initialize information for a new thread (with context  
creation), yield execution from running thread to first thread on `ready`  
queue and return the thread ID of the running thread.

In `uthread_exit`, inspired by the Piazza post @347 that we asked, we learn  
that it is used to put a dead thread to `zombie` queue and collect return  
value, put the blocked thread (parent thread of the dead thread) on `ready`  
and yield the next available thread on`ready` to run. The `ptid` data in  
thread control block comes in useful because we can use it to find the  
blocked parent thread in`blocked` queue using `find_tid`. Also, we do  
unblocking process first because it can signal the fact that `ready` queue is  
not empty, so we have to execute it. 

In `uthread_join`, we learn that if the (child) thread with parameter `tid`  
is found in `ready`, then we have to yield execution of the running (parent)  
thread to the child thread and block the parent (put on `blocked` queue). We  
store the parent thread ID in child's `ptid` and mark as `joined = 1` so we  
can know that this thread has been joined by a parent thread (useful for  
`uthread_exit`). If child thread is found in `zombie`, we collect its return  
value and free resources of it. If found nowhere, we return -1. 

## Phase 4: Preempt.c
We learn that in case the thread does not want to yield and wants to run  
forever, we have to force it with preemption. That is why we have a helper  
function `signal_handler` of yield to do this. In `preempt_start`, we set up  
a virtual alarm with such signal handler and fire the alarm so that the  
thread is not allowed to run more than 0.01 seconds (1s/100HZ), from  
0-999,999 microseconds. When `preempt_stop` is called, we reset the virtual  
timer to 0 and reset signal handler and flags to defaults of `man` page.

In `preempt_enable` and `preempt_disable` we merely initalize signal mask  
and use `sigprocmask` to unblock or block the lock for critical section.

All the global variables related to this phase is put in `static` to avoid  
exposure to user threads.

## Preemption in uthread.c
We learn the use of preemption enabling is to lock/unblock the critical  
section - where global variables are in danger of being modified by  
concurrency. Therefore, we use `preempt_disable` to lock the critical section  
where global variable is being used, and unblock it using `preempt_enable`  
when no global variables are involved.

## Testing
We have the tester files `queue_tester.c`, `uthread_hello_tester.c` and  
`uthread_yield_tester.c`, along with other provided tester files by the  
professor to test the program. We make sure it passes all, including corner  
cases of NULL values where we have faced `BAD_ACCESS` or cast errors. 

### test_preempt.c
Understanding that preemption comes in handy when threads do not yield and  
want to run forever, we remove all the `uthread_yield` call and put the  
threads in infinite loop. We receive the result below in our CSIF:
```
$./test_preempt.x
Hello!
Bye!
```
This is the case where we allow the threads to be executed in 0.01 seconds.

## Limitations
With the time spent concurrently on the project and the midterm and long  
debugging and reworking processes due to failing cases, we run out of time  
to include more complicated test cases. We apologize for this and will make  
sure to understand what can go wrong in our program (if any) for the  
interactive grading.

Also, instead of using `ptid`, we could have just stored the parent thread in  
a struct pointer of the child's thread control block. Doing this could have  
saved us time complexity O(1) from iterating O(n) through each thread of the  
`blocked` queue.

## References
- Lecture 03.syscalls (for setting up alarm)
- Lecture 06.process-scheduling (for general picture of thread processes & yield)
- Lecture 07.concurrency-threads (for understanding concurrency and preemption)
- https://www.geeksforgeeks.org/queue-linked-list-implementation/
(for create & enqueue)
- https://www.codesdope.com/blog/article/making-a-queue-using-linked-list-in-c/
(for dequeue as above fails)
- https://stackoverflow.com/questions/17080077/deleting-a-node-from-a-queue
(for delete)
- https://www.gnu.org/software/libc/manual/html_mono/libc.html#Signal-Actions
(for setting up signal handler)
- https://www.gnu.org/software/libc/manual/html_mono/libc.html#Setting-an-Alarm
(for start & stop preemption)
- https://man7.org/linux/man-pages/man2/sigaction.2.html
(for defaults of signal handler & flags)
- https://www.gnu.org/software/libc/manual/html_mono/libc.html#Blocking-Signals (24.7.4)
(for initializing signal mask and block/unblock signaling feature)
- Piazza posts @347, @383, @440, @451, @454
