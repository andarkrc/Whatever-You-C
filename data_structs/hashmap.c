#include "hashmap.h"

map_t *map_create_generic(void (*data_destroy)(void *), void (*key_destroy)(void *), size_t (*hash)(void *), int (*keycmp)(void *, void *), void *(*keycpy)(void *))
{
	map_t *map = malloc(sizeof(*map));
	ASSERT(map);
	map->capacity = 128;
	map->size = 0;
	map->buckets = calloc(128, sizeof(*map->buckets));
	ASSERT(map->buckets);
	map->data_destroy = data_destroy;
	map->key_destroy = key_destroy;
	map->hash = hash;
	map->keycmp = keycmp;
	map->keycpy = keycpy;

	return map;
}

static void map_recalibrate_up(map_t *map)
{
	size_t new_capacity;
	if (map->size == map->capacity * 2) {
		new_capacity = map->size;
	} else {
		return;
	}
	map_t *new_map = map_create_generic(map->data_destroy, map->key_destroy, map->hash, map->keycmp, map->keycpy);
	free(new_map->buckets);
	new_map->buckets = calloc(new_capacity, sizeof(*new_map->buckets));
	ASSERT(new_map->buckets);
	new_map->capacity = new_capacity;
	for (size_t i = 0; i < map->capacity; i++) {
		while (map->buckets[i]) {
			entry_t *entry = map->buckets[i];
			map->buckets[i] = entry->next;
			map_add(new_map, entry->key, entry->data);
			if (map->key_destroy) {
				map->key_destroy(entry->key);
			}
			free(entry);
		}
	}
	free(map->buckets);
	map->buckets = new_map->buckets;
	map->capacity = new_capacity;
	free(new_map);
}

static void map_recalibrate_down(map_t *map)
{
	size_t new_capacity;
	if (map->size == map->capacity / 2) {
		new_capacity = map->size;
	} else {
		return;
	}
	map_t *new_map = map_create_generic(map->data_destroy, map->key_destroy, map->hash, map->keycmp, map->keycpy);
	free(new_map->buckets);
	new_map->buckets = calloc(new_capacity, sizeof(*new_map->buckets));
	ASSERT(new_map->buckets);
	new_map->capacity = new_capacity;
	for (size_t i = 0; i < map->capacity; i++) {
		while (map->buckets[i]) {
			entry_t *entry = map->buckets[i];
			map->buckets[i] = entry->next;
			map_add(new_map, entry->key, entry->data);
			if (map->key_destroy) {
				map->key_destroy(entry->key);
			}
			free(entry);
		}
	}
	free(map->buckets);
	map->buckets = new_map->buckets;
	map->capacity = new_capacity;
	free(new_map);
}

void map_add(map_t *map, void *key, void *data)
{
	size_t index = map->hash(key) % map->capacity;
	entry_t *old_entry = map->buckets[index];
	while (old_entry) {
		if (map->keycmp(old_entry->key, key) == 0) {
			break;
		}
		old_entry = old_entry->next;
	}

	if (!old_entry) {
		entry_t *entry = malloc(sizeof(*entry));
		ASSERT(entry);
		entry->next = map->buckets[index];
		map->buckets[index] = entry;
		entry->data = data;
		entry->key = map->keycpy(key);
		map->size++;
	} else {
		if (map->data_destroy) {
			map->data_destroy(old_entry->data);
		}
		old_entry->data = data;
	}
	map_recalibrate_up(map);
}

void *map_remove(map_t *map, void *key)
{
	size_t index = map->hash(key) % map->capacity;
	entry_t *prev = NULL;
	entry_t *entry = map->buckets[index];
	while (entry) {
		if (map->keycmp(entry->key, key) == 0) {
			if (prev == NULL) {
				map->buckets[index] = entry->next;
			} else {
				prev->next = entry->next;
			}
			if (map->key_destroy) {
				map->key_destroy(entry->key);
				void *data = entry->data;
				free(entry);
				map->size--;
				map_recalibrate_down(map);
				return data;
			}
		}
		prev = entry;
		entry = entry->next;
	}

	return NULL;
}

void map_purge(map_t *map, void *key)
{
	void *data = map_remove(map, key);
	if (!data) {
		return;
	}
	if (map->data_destroy) {
		map->data_destroy(data);
	}
}

void *map_get(map_t *map, void *key)
{
	size_t index = map->hash(key) % map->capacity;

	entry_t *entry = map->buckets[index];
	while (entry)
	{
		if (map->keycmp(entry->key, key) == 0)
		{
			return entry->data;
		}
		entry = entry->next;
	}

	return NULL;
}

void map_destroy(map_t *map)
{
	for (size_t i = 0; i < map->capacity; i++)
	{
		while (map->buckets[i])
		{
			entry_t *entry = map->buckets[i];
			map->buckets[i] = entry->next;

			if (map->key_destroy) {
				map->key_destroy(entry->key);
			}
			if (map->data_destroy)
			{
				map->data_destroy(entry->data);
			}
			free(entry);
		}
	}
	free(map->buckets);
	free(map);
}

size_t hash_string(void *key)
{
	unsigned char *key_string = (unsigned char *)key;
	size_t hash = 5381;
	size_t c;

	while ((c = *key_string++))
		hash = ((hash << 5u) + hash) + c;

	return hash;
}

int strkeycmp(void *key1, void *key2)
{
	return strcmp(key1, key2);
}

size_t strkeylen(void *key) {
	return strlen(key) + 1;
}

void *strkeycpy(void *key)
{
	size_t len = strlen(key) + 1;
	void *new_key = malloc(len);
	ASSERT(new_key);
	memcpy(new_key, key, len);

	return new_key;
}