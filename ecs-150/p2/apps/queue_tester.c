#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <queue.h>

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


static int inc_item(queue_t q, void *data, void *arg)
{
   	int *a = (int*)data;
   	int inc = (int)(long)arg;

   	if (*a == 42)
		queue_delete(q, data);
	else
		*a += inc;
	
	return 0;
}

/* Create */
void test_create(void)
{
	fprintf(stderr, "*** TEST create ***\n");

	TEST_ASSERT(queue_create() != NULL);
}

/* Enqueue/Dequeue simple */
void test_queue_simple(void)
{
	int data = 3, *ptr;
	queue_t q;

	fprintf(stderr, "*** TEST queue_simple ***\n");

	q = queue_create();
	queue_enqueue(q, &data);
	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(ptr == &data);
}

// my test case
void test_queue_length(void)
{
	int data0 = 1;
	int data1 = 9;
	int data2 = 9;
	int data3 = 8;
	queue_t q;

	fprintf(stderr, "*** TEST queue_length ***\n");

	q = queue_create();
	queue_enqueue(q, &data0);
	queue_enqueue(q, &data1);
	queue_enqueue(q, &data2);
	queue_enqueue(q, &data3);
	
	TEST_ASSERT(queue_length(q) == 4);
}

// my test case
void test_queue_delete(void)
{
	int data0 = 1;
	int data1 = 9;
	int data2 = 9;
	int data3 = 8;
	int data4 = 10;
	int data5 = 11;
	queue_t q;

	fprintf(stderr, "*** TEST queue_delete ***\n");

	q = queue_create();
	queue_enqueue(q, &data0);
	queue_enqueue(q, &data1);
	queue_enqueue(q, &data2);
	queue_enqueue(q, &data3);

	queue_delete(q, &data3);
	
	TEST_ASSERT(queue_length(q) == 3);

	TEST_ASSERT(queue_delete(q, &data2) == 0);
	TEST_ASSERT(queue_delete(q, &data1) == 0);

	TEST_ASSERT(queue_length(q) == 1);

	TEST_ASSERT(queue_delete(q, &data0) == 0);

	TEST_ASSERT(queue_length(q) == 0);

	queue_enqueue(q, &data5);

	TEST_ASSERT(queue_length(q) == 1);

	TEST_ASSERT(queue_delete(q, NULL) == -1);
	TEST_ASSERT(queue_delete(q, &data4) == -1);
	TEST_ASSERT(queue_delete(q, &data3) == -1);
	TEST_ASSERT(queue_delete(q, &data2) == -1);

}

// my test case
void test_queue_destroy(void)
{
	int data0 = 1, *ptr0;
	int data1 = 9, *ptr1;
	int data2 = 9, *ptr2;
	int data3 = 8, *ptr3;
	queue_t q;

	fprintf(stderr, "*** TEST queue_destroy***\n");

	q = queue_create();
	queue_enqueue(q, &data0);
	queue_enqueue(q, &data1);
	queue_enqueue(q, &data2);
	queue_enqueue(q, &data3);
	int result = queue_destroy(q);
	
	TEST_ASSERT(result == -1);

	fprintf(stderr, "*** TEST queue_destroy_when_it_is_zero***\n");

	queue_dequeue(q, (void**)&ptr0);
	queue_dequeue(q, (void**)&ptr1);
	int result0 = queue_length(q);
	TEST_ASSERT(result0 == 2);
   	queue_dequeue(q, (void**)&ptr2);
	queue_dequeue(q, (void**)&ptr3);

	int check = queue_length(q);
	TEST_ASSERT(check == 0);

   	int result1 = queue_destroy(q);
	TEST_ASSERT(result1 == 0);
}

// my test case
void test_queue_iterate(void)
{
	char data0[32] = "1";
	char data1[32] = "9";
	char data2[32] = "9";
	char data3[32] = "8";

	queue_t q;
	// queue_func_t func;

	q = NULL;

	// void *arg;
	// arg = &8;
	void **data = NULL;

	fprintf(stderr, "*** TEST queue_iterate when queue is null***\n");
	int result1 = queue_iterate(q, inc_item, &"1998", data);
	TEST_ASSERT(result1 == -1);

	fprintf(stderr, "*** TEST queue_iterate***\n");
	queue_t Q;

	Q = queue_create();
	queue_enqueue(Q, &data0);
	queue_enqueue(Q, &data1);
	queue_enqueue(Q, &data2);
	queue_enqueue(Q, &data3);

	void *arg = NULL;
	void *arg1 = &"1";
	
	void **newdata = &arg1;

	int result2 = queue_iterate(Q, inc_item, arg, newdata);
	
	TEST_ASSERT(result2 == 0);

}

int main(void)
{
	test_create();
	test_queue_simple();
	test_queue_length();
	test_queue_delete();
	test_queue_destroy();
	test_queue_iterate();

	return 0;
}
