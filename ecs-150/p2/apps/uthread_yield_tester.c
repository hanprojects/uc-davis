/*
 * Thread creation and yielding test
 *
 * Tests the creation of multiples threads and the fact that a parent thread
 * should get returned to before its child is executed. The way the printing,
 * thread creation and yielding is done, the program should output:
 *
 * thread1
 * thread2
 * thread3
 */

#include <stdio.h>
#include <stdlib.h>

#include "uthread.h"

#define TEST_ASSERT(assert)			\
do {						\
	printf("ASSERT: " #assert " ... ");	\
	if (assert) {				\
		printf("PASS\n");		\
	} else	{				\
		printf("FAIL\n");		\
		exit(1);			\
	}					\
} while(0)

int thread3(void)
{
	uthread_yield();
	printf("thread%d\n", uthread_self());
	return 0;
}

int thread2(void)
{
	uthread_create(thread3);
	uthread_yield();
	printf("thread%d\n", uthread_self());
	return 0;
}

int thread1(void)
{
	uthread_create(thread2);
	uthread_yield();
	printf("thread%d\n", uthread_self());
	uthread_yield();
	return 0;
}

int main(void)
{
	// prof case
	uthread_start(0);
	uthread_create(thread1);
	uthread_stop();

	// my case
	uthread_t check = 0;
	TEST_ASSERT(uthread_start(0) == 0);
	TEST_ASSERT(uthread_join(uthread_create(thread1), NULL) == 0);
	TEST_ASSERT(uthread_join(uthread_create(thread2), NULL) == 0);
	TEST_ASSERT(uthread_self() == check);
	TEST_ASSERT(uthread_join(uthread_create(thread3), NULL) == 0);
	TEST_ASSERT(uthread_stop() == 0);

	return 0;
}
