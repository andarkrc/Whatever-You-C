#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

struct entry_t {
	struct entry_t *next;
	void *data;
	void *key;
};

typedef struct entry_t entry_t;

typedef struct {
	entry_t **buckets;
	size_t capacity;
	size_t size;
	void (*data_destroy)(void *);
	void (*key_destroy)(void *);
	size_t (*hash)(void *);
	int (*keycmp)(void *, void*);
	void *(*keycpy)(void *);
} hashmap_t;

typedef hashmap_t map_t;

size_t hash_string(void *key);
int strkeycmp(void *key1, void *key2);
void *strkeycpy(void *key);

map_t *map_create_generic(void (*data_destroy)(void *), void (*key_destroy)(void *), size_t (*hash)(void *), int (*keycmp)(void *, void *), void *(*keycpy)(void *));
void map_destroy(map_t *map);
void map_add(map_t *map, void *key, void *data);
void *map_remove(map_t *map, void *key);
void *map_get(map_t *map, void *key);
void map_purge(map_t *map, void *key);

#define map_create(data_destroy) map_create_generic(data_destroy, free, hash_string, strkeycmp, strkeycpy)
#define map_put(map, key, data) map_add(map, key, data)
#define map_has(map, key) (map_get(map, key) != NULL)
#define map_contains(map, key) (map_get(map, key) != NULL)
#define hashmap_create_generic(data_destroy, key_destroy, hash, keycmp, keycpy) map_create_generic(data_destroy, key_destroy, hash, strkeycmp, keycpy)
#define hashmap_create(data_destroy) map_create(data_destroy)
#define hashmap_add(map, key, data) map_add(map, key, data)
#define hashmap_put(map, key, data) map_add(map, key, data)
#define hashmap_get(map, key) map_get(map, key)
#define hashmap_has(map, key) map_has(map, key)
#define hashmap_contains(map, key) map_has(map, key)
#define hashmap_remove(map, key) map_remove(map, key)
#define hashmap_destroy(map) map_destroy(map)
#define hashmap_purge(map, key) map_purge(map, key) 

#endif
