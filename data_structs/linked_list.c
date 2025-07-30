#include "linked_list.h"

list_t *list_create(void (*data_destroy)(void *))
{
	list_t *list = malloc(sizeof(*list));
	ASSERT(list);
	list->size = 0;
	list->head = list->tail = NULL;
	list->data_destroy = data_destroy;

	return list;
}

void list_add(list_t *list, size_t index, void *data)
{
	if (index > list->size) {
		index = list->size;
	}

	node_t *node = malloc(sizeof(*node));
	ASSERT(node);
	node->next = node->prev = NULL;
	node->data = data;

	if (list_empty(list)) {
		list->head = list->tail = node;
		list->size++;
		return;
	}

	if (index == 0) {
		node->next = list->head;
		list->head->prev = node;
		list->head = node;
		list->size++;
		return;
	}

	if (index == list->size) {
		node->prev = list->tail;
		list->tail->next = node;
		list->tail = node;
		list->size++;
		return;
	}

	index--;
	node_t *prev = list->head;
	while (index) {
		prev = prev->next;
		index--;
	}

	node->next = prev->next;
	prev->next = node;
	node->prev = prev;
	if (node->next) {
		node->next->prev = node;
	} else {
		list->tail = node;
	}
	list->size++;
}

void *list_get(list_t *list, size_t index)
{
	if (index >= list->size) {
		return NULL;
	}

	if (index == 0) {
		return list->head->data;
	}

	if (index == list->size - 1) {
		return list->tail->data;
	}

	node_t *node = list->head;
	while (index) {
		node = node->next;
		index--;
	}

	return node->data;
}

void *list_remove(list_t *list, size_t index)
{
	if (index >= list->size) {
		return NULL;
	}

	if (list->size == 1) {
		void *data = list->head->data;
		free(list->head);
		list->head = list->tail = NULL;
		list->size--;
		return data;
	}

	if (index == 0) {
		void *data = list->head->data;
		list->head->next->prev = NULL;
		node_t *head = list->head;
		list->head = head->next;
		free(head);
		list->size--;
		return data;
	}

	if (index == list->size - 1) {
		void *data = list->tail->data;
		list->tail->prev->next = NULL;
		node_t *tail = list->tail;
		list->tail = tail->prev;
		free(tail);
		list->size--;
		return data;
	}

	node_t *current = list->head;
	while (index) {
		current = current->next;
		index--;
	}

	node_t *prev = current->prev;
	node_t *next = current->next;

	prev->next = current->next;
	if (next) {
		next->prev = prev;
	} else {
		list->tail = prev;
	}
	list->size--;
	void *data = current->data;
	free(current);
	return data;
}

void list_purge(list_t *list, size_t index)
{
	void *data = list_remove(list, index);
	if (list->data_destroy != NULL) {
		list->data_destroy(data);
	}
}

void list_destroy(list_t *list)
{
	while (!list_empty(list)) {
		void *data = list_remove(list, 0);
		if (list->data_destroy != NULL) {
			list->data_destroy(data);
		}
	}
	free(list);
}

void list_traversefw(list_t *list, void *acc, void (*action)(void *, void *))
{
	node_t *current = list->head;
	while (current) {
		action(current->data, acc);
		current = current->next;
	}
}

void list_traversebw(list_t *list, void *acc, void (*action)(void *, void *))
{
	node_t *current = list->tail;
	while (current) {
		action(current->data, acc);
		current = current->prev;
	}
}

// LIST INLINES

void list_push(list_t *list, void *data)
{
	list_add(list, list->size, data);
}

void list_pushback(list_t *list, void *data)
{
	list_add(list, list->size, data);
}

void list_pushfront(list_t *list, void *data)
{
	list_add(list, 0, data);
}

void *list_pop(list_t *list)
{
	list_remove(list, list->size - 1);
}

void *list_popback(list_t *list)
{
	list_remove(list, list->size - 1);
}

void *list_popfront(list_t *list)
{
	list_remove(list, 0);
}

char list_empty(list_t *list)
{
	return (list->size == 0);
}

void *list_front(list_t *list)
{
	return (list->head != NULL) ? list->head->data : NULL;
}

void *list_back(list_t *list)
{
	return (list->tail != NULL) ? list->tail->data : NULL;
}

// QUEUE INLINES

queue_t *queue_create(void (*data_destroy)(void *))
{
	return list_create(data_destroy);
}

void queue_destroy(queue_t *q)
{
	list_destroy(q);
}

void queue_push(queue_t *q, void *data)
{
	list_add(q, q->size - 1, data);
}

void *queue_pop(queue_t *q)
{
	return list_remove(q, 0);
}

void *queue_peek(queue_t *q)
{
	return (q->head != NULL) ? q->head->data : NULL;
}

char queue_empty(queue_t *q)
{
	return (q->size == 0);
}

void queue_traverse(queue_t *q, void *acc, void (*action)(void *, void *))
{
	node_t *current = q->head;
	while (current) {
		action(current->data, acc);
		current = current->next;
	}
}

// STACK INLINES

stack_t *stack_create(void (*data_destroy)(void *))
{
	return list_create(data_destroy);
}

void stack_destroy(stack_t *stack)
{
	list_destroy(stack);
}

void stack_push(stack_t *stack, void *data)
{
	list_add(stack, stack->size - 1, data);
}

void *stack_pop(stack_t *stack)
{
	return list_remove(stack, stack->size - 1);
}

void *stack_peek(stack_t *stack)
{
	return (stack->tail != NULL) ? stack->tail->data : NULL;
}

char stack_empty(stack_t *stack)
{
	return (stack->size == 0);
}

void stack_traverse(stack_t *stack, void *acc, void (*action)(void *, void *))
{
	node_t *current = stack->tail;
	while (current) {
		action(current->data, acc);
		current = current->prev;
	}
}
