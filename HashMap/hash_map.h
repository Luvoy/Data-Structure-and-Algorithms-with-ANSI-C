#include "../my_bool.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#ifndef _HASH_MAP_H_
#define _HASH_MAP_H

typedef struct KV
{
    void *key;
    void *value;

} KV;

typedef struct HashNode
{
    struct HashNode *next;
    KV *data;
} HashNode;

typedef struct HashMap
{
    size_t total_size;
    size_t used_size;
    const char *hash_algo;
    HashNode *maps;
} HashMap;
extern my_bool key_equals(const void *key1, const void *key2);
extern uint32_t murmur_hash(const void *p);
extern uint32_t BKDR_hash(const void *p);
extern uint32_t hash_code(const void *p, const char *hash_algo);
extern HashMap *hash_map_new(size_t n, const char *hash_algo);
extern void hash_map_free(HashMap **hm);
extern HashMap *hash_map_extend(HashMap *hm);
extern void *hash_map_put_kv(HashMap **hm, void *key, void *value);

extern void *hash_map_get(HashMap *hm, void *key);

extern my_bool hash_map_contains(HashMap *hm, const void *key);
extern void *hash_map_remove(HashMap *hm, void *key);
extern void hash_map_print(HashMap *hm, FILE *f, void (*print_key)(FILE *, const void *), void (*print_value)(FILE *, const void *));
#endif
