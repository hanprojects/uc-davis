#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

// Linked list to store info about data item
typedef struct queue_data_item
{
        void *data;
        struct queue_data_item *next;
}
item;

// Struct queue
struct queue
{
        int length;
        struct queue_data_item *head;
        struct queue_data_item *tail;
};

// queue_create() - must be O(1)
// Source: https://www.geeksforgeeks.org/queue-linked-list-implementation/
queue_t queue_create(void)
{
        queue_t new_queue = malloc(sizeof(struct queue));
        
        // If allocated successfully, initialize info in queue
        // and return pointer of new queue
        if (new_queue)
        {
                new_queue->head = NULL;
                new_queue->tail = NULL;
                new_queue->length = 0;
                
                return new_queue;
        }

	return NULL;
}

// queue_destroy() - must be O(1)
int queue_destroy(queue_t queue)
{
	if (queue == NULL || queue->length != 0)
        {
                return -1;
        }

        // Destroy empty queue
        free(queue);
        return 0;
}

// queue_enqueue() - must be O(1)
// Source: https://www.geeksforgeeks.org/queue-linked-list-implementation/
int queue_enqueue(queue_t queue, void *data)
{
        if (queue == NULL || data == NULL)
        {
	        return -1;
        }

        item *new_item = malloc(sizeof(struct queue_data_item));
        
        // Check the case of memory allocation error when enqueuing
        if (!new_item)
        {
                return -1;
        }

        new_item->data = data;
        new_item->next = NULL;

        // If queue is empty,
        // add new data item to both queue's head & tail
        if (queue->tail == NULL)
        {
                queue->head = queue->tail = new_item;
        }

        // Else, set tail next to item
        else
        {
                queue->tail->next = new_item;
                queue->tail = new_item;
        }

        queue->length += 1;
        return 0;
}

// queue_dequeue() - must be O(1)
// Source: https://www.codesdope.com/blog/article/making-a-queue-using-linked-list-in-c/
int queue_dequeue(queue_t queue, void **data)
{
        if (queue == NULL || data == NULL || queue->length == 0)
        {
		return -1;
	}

        *data = queue->head->data;

        // If only one node, free head
        if (queue->length == 1)
        {
                free(queue->head);
                queue->head = queue->tail = NULL;

                queue->length -= 1;
                return 0;
        }

        // Else, save value of head->next before free(head)
        // and move head to head->next
        item *saved_head_next = queue->head->next;
        free(queue->head);
        queue->head = saved_head_next;

	queue->length -= 1;
	return 0;
}

// queue_delete() - no need for O(1)
// Source: https://stackoverflow.com/questions/17080077/deleting-a-node-from-a-queue
int queue_delete(queue_t queue, void *data)
{
        if (queue == NULL || data == NULL || queue->length == 0) // avoid BAD_ACCESS
        {
                return -1;
        }

        // If data is right at the head
        // move head to head->next and return
        if (queue->head->data == data)
        {
                queue->head = queue->head->next;
                queue->length -= 1;
                return 0;
        }

        // Else, loop through the queue
        // and stop at matching data
        item *curr = queue->head;
        item *prev = NULL;

        while (curr != NULL && curr->data != data)
        {
                prev = curr;
                curr = curr->next;
        }

        // If end of queue and no data found
        // return -1
        if (curr == NULL)
        {
                return -1;
        }

        // Else, merge previous node to curr node
        else
        {
                prev->next = curr->next;
                queue->length -= 1;
                return 0;
        }
}

// queue_iterate() - no need for O(1)
int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data)
{
        if (queue == NULL || data == NULL)
        {
                return -1;
        }

        item *curr = queue->head;
        item *next = NULL;

        // Start iterating through queue
        while (curr)
        {
                // Feature for deletion resistance
                // Source: Piazza @383
                next = curr->next;

                if (func(queue, curr->data, arg) == 1)
                {
                        if (data != NULL)
                        {
                                // store data where loop stops
                                *data = curr->data;
                        }
                        return 0;
                }

                curr = next;
        }

        return 0;
}

// queue_length() - must be O(1)
int queue_length(queue_t queue)
{
        if (queue == NULL)
        {
                return -1;
        }

	return queue->length;
}
