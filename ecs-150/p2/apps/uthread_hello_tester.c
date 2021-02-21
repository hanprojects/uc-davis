/*
 * Simple hello world test
 *
 * Tests the creation of a single thread and its successful return.
 */

#include <stdio.h>
#include <stdlib.h>

#include "uthread.h"

#define USHRT_MAX (__SHRT_MAX__ *2  +1)

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

int hello(void)
{
	printf("Hello worrrrld!\n");
	return 0;
}

int bye(void)
{
	printf("Bye world!\n");
	return 1;
}

int main(void)
{
	// prof's version
	uthread_t tid;

	uthread_start(0);
	uthread_create(hello);
	uthread_join(tid, NULL);
	uthread_stop();

	// my version
	uthread_t tid1;
	uthread_t num = -1;
	// start -- create -- TEST --join -- self
	TEST_ASSERT(uthread_start(0) == 0);
	tid1 = uthread_create(hello);
	TEST_ASSERT(tid1 != num);

	TEST_ASSERT(uthread_self() == 0);
	TEST_ASSERT(uthread_join(tid1, 0) == 0);
	uthread_yield();
	TEST_ASSERT(uthread_self() == 0);

	TEST_ASSERT(uthread_stop() == 0);

	return 0;
}
