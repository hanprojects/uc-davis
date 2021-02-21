#include <stdio.h>
#include <stdlib.h>

#include <uthread.h>

// Remove all uthread_yield() to test the context switching
// between threads without calling yields when preempted

int see_you_again(void)
{
        while (1)
        {
                printf("See you again!\n");
        }

        return 0;
}

int bye(void)
{
        uthread_create(see_you_again);

        while (1)
        {
                printf("Bye!\n");
        }

        return 0;
}

int hello(void)
{
        uthread_create(bye);

        while (1)
        {
	        printf("Hello!\n");
        }

        return 0;
}

int main(void)
{
        uthread_t tid;

        uthread_start(1);
        tid = uthread_create(hello);
        uthread_join(tid, NULL);
        uthread_stop();

        return 0;
}