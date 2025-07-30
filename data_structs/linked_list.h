#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

struct node_t {
	struct node_t *next;
	struct node_t *prev;
	void *data;	
};

typedef struct node_t node_t;

typedef struct {
	node_t *head;
	node_t *tail;
	size_t size;
	void (*data_destroy)(void *);
} list_t;

typedef list_t queue_t;
typedef list_t stack_t;

list_t *list_create(void (*data_destroy)(void *));
void list_destroy(list_t *list);
void list_add(list_t *list, size_t index, void *data);
void *list_get(list_t *list, size_t index);
void *list_remove(list_t *list, size_t index);
void list_purge(list_t *list, size_t index);
void list_traversefw(list_t *list, void *acc, void (*action)(void *, void *));
void list_traversebw(list_t *list, void *acc, void (*action)(void *, void *));
void list_push(list_t *list, void *data);
void list_pushback(list_t *list, void *data);
void list_pushfront(list_t *list, void *data);
void *list_pop(list_t *list);
void *list_popback(list_t *list);
void *list_popfront(list_t *list);
char list_empty(list_t *list);
void *list_front(list_t *list);
void *list_back(list_t *list);


queue_t *queue_create(void (*data_destroy)(void *));
void queue_destroy(queue_t *q);
void queue_push(queue_t *q, void *data);
void *queue_pop(queue_t *q);
void *queue_peek(queue_t *q);
char queue_empty(queue_t *q);
void queue_traverse(queue_t *q, void *acc, void (*action)(void *, void *));

stack_t *stack_create(void (*data_destroy)(void *));
void stack_destroy(stack_t *stack);
void stack_push(stack_t *stack, void *data);
void *stack_pop(stack_t *stack);
void *stack_peek(stack_t *stack);
char stack_empty(stack_t *stack);
void stack_traverse(stack_t *stack, void *acc, void (*action)(void *, void *));


#endif
