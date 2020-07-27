#include "hash_map.h"
#include "../str_func.h"
#include <assert.h>
#include <string.h>
#ifndef NODE_ADD_ALGORITHM
#define NODE_ADD_ALGORITHM
#endif
#ifdef NODE_ADD_ALGORITHM
#define NODE_ADD_HAED
#define NODE_ADD_TAIL
#endif
#ifdef NODE_ADD_TAIL
#undef NODE_ADD_HAED
#endif
#ifdef NODE_ADD_HAED
#undef NODE_ADD_TAIL
#endif

#ifndef HASH_MAP_DEBUG
#define HASH_MAP_DEBUG
#endif
extern my_bool key_equals(const void *key1, const void *key2)
{
    if (key1 == NULL && key2 != NULL)
    {
        return FALSE;
    }
    if (key1 != NULL && key2 == NULL)
    {
        return FALSE;
    }
    if (key1 == NULL && key2 == NULL)

    {
        return TRUE;
    }
    return !strcmp((const char *)key1, (const char *)key2);
}

extern uint32_t murmur_hash(const void *p)
{
    char *str = (char *)p;
    size_t len;
    while (*str++)
    {
        ++len;
    }
    uint32_t h, k;
    h = 0 ^ len;
    while (len >= 4)
    {
        k = str[0];
        k |= str[1] << 8;
        k |= str[2] << 16;
        k |= str[3] << 24;

        k *= 0x5bd1e995;
        k ^= k >> 24;
        k *= 0x5bd1e995;

        h *= 0x5bd1e995;
        h ^= k;

        str += 4;
        len -= 4;
    }
    switch (len)
    {
    case 3:
        h ^= str[2] << 16;
    case 2:
        h ^= str[1] << 8;
    case 1:
        h ^= str[0];
        h *= 0x5bd1e995;
    }
    h ^= h >> 13;
    h *= 0x5bd1e995;
    h ^= h >> 15;
    return h;
}

extern uint32_t BKDR_hash(const void *p)
{
    char *str = (char *)p;
    uint32_t seed = 131; /* 31 131 1313 13131 131313 etc..*/
    uint32_t hash = 0;
    while (*str)
    {
        hash = hash * seed + (*str++);
    }
    return (hash & 0x7FFFFFFF);
}

extern uint32_t hash_code(const void *str, const char *hash_algo)
{
    assert(str);
    if (strcmp(hash_algo, "murmur") == 0)
    {
        return murmur_hash(str);
    }
    else if (strcmp(hash_algo, "BKDR") == 0)
    {
        return BKDR_hash(str);
    }
    else
    {
        return BKDR_hash(str);
    }
}

extern HashMap *hash_map_new(size_t n, const char *hash_algo)
{
    if (n < 0)
    {
        fprintf(stderr, "size is negative!");
    }
    size_t temp = 1;
    while (temp < n)
    {
        temp <<= 1;
    } /* 让hashmap的总容量为2的次幂*/

    HashMap *hm = (HashMap *)calloc(1, sizeof(HashMap));
    assert(hm);
    hm->total_size = temp + 1; /* 给null留一个*/
    hm->used_size = 0;
    hm->hash_algo = hash_algo;
    hm->maps = (HashNode *)calloc(hm->total_size, sizeof(HashNode));
    assert(hm->maps);
    size_t i;
    for (i = 0; i < hm->total_size; ++i)
    {
        (hm->maps + i)->next = NULL;
        (hm->maps + i)->data = (KV *)calloc(1, sizeof(KV));
        assert((hm->maps + i)->data);
    }
    return hm;
}

extern void hash_map_free(HashMap **hm)
{
    size_t i;
    HashNode *p_second;
    HashNode *p_last;
    for (i = 0; i < (*hm)->total_size; ++i)
    {
        p_last = (*hm)->maps + i;
        while (p_last->next)
        {
            p_second = p_last;
            p_last = p_last->next;
            if (p_second != (*hm)->maps + i)
            { /*如果倒数第二个指针指向的不是头，那么释放它*/
                free(p_second->data);
                p_second->data = NULL;
                free(p_second);
            }
        }
        p_second = NULL;
        if (p_last != (*hm)->maps + i)
        { /*如果最后一个节点不是第一个（也就是尾指针移动了）*/
            /*释放尾*/
            free(p_last->data);
            p_last->data = NULL;
            free(p_last);
            p_last = NULL;
        }

        /*释放头*/
        free(((*hm)->maps + i)->data);
        ((*hm)->maps + i)->data = NULL;
        if (i != 0)
        {
            free((*hm)->maps + i);
        }
    }
    free((*hm)->maps);
    (*hm)->maps = NULL;
    free(*hm);
    *hm = NULL;
}

extern HashMap *hash_map_extend(HashMap *hm)
{
    HashMap *hm_new = hash_map_new(hm->total_size, hm->hash_algo); /*这里直接传入hm的总size即可，因为size一直是2的次幂+1，会自动*2并且+1*/
    HashNode *entry;
    uint32_t hash_index;
    for (hash_index = 0; hash_index < hm->total_size; hash_index++)
    {
        for (entry = hm->maps + hash_index; entry != NULL; entry = entry->next)
        {
            hash_map_put_kv(&hm_new, entry->data->key, entry->data->value);
        }
    }
    hash_map_free(&hm);
    return hm_new;
}

extern void *hash_map_put_kv(HashMap **hm, void *key, void *value)
{
    assert(hm);
    assert((*hm));
    void *ret = NULL;

    if ((double)((*hm)->used_size + 1) / (double)((*hm)->total_size) > 0.75)
    {
        (*hm) = hash_map_extend((*hm));
    }
    uint32_t index;
    if (key != NULL)
    {
        uint32_t hash = hash_code(key, (*hm)->hash_algo);
        index = (hash & ((*hm)->total_size - 2)) + 1;
#ifdef HASH_MAP_DEBUG
        fprintf(stdout, "hash of key: %u, index: %u\n", hash, index);
#endif
    }
    else
    {
        index = 0;
        ret = ((*hm)->maps + index)->data->value;
        if (ret == NULL)
        {
            if (value == NULL)
            {
                return NULL;
            }
            ((*hm)->maps + index)->data->key = NULL;
            ((*hm)->maps + index)->data->value = value;
            (*hm)->used_size++;
            return ret;
        }
        else
        {
            if (value == NULL)
            {
                (*hm)->used_size--;
            }
            ((*hm)->maps + index)->data->value = value;
            return ret;
        }
    }
    HashNode *entry;
    if (((*hm)->maps + index)->data->key == NULL)
    {
        /* 空, 直接插入*/
        ret = ((*hm)->maps + index)->data->value;
        ((*hm)->maps + index)->data->key = key;
        ((*hm)->maps + index)->data->value = value;
        (*hm)->used_size++;
        return ret;
    }
#ifdef NODE_ADD_TAIL
    HashNode *temp = NULL; /*比entry慢一个*/
#endif
    for (entry = (*hm)->maps + index; entry != NULL; entry = entry->next)
    {
#ifdef NODE_ADD_TAIL
        temp = entry; /*比entry慢一个*/
#endif
        if (entry->data->key == key || key_equals(entry->data->key, key))
        { /*已有 ,直接覆盖*/
            ret = entry->data->value;
            entry->data->value = value;
            return ret;
        }
    }

    HashNode *new_entry = (HashNode *)calloc(1, sizeof(HashNode));
    assert(new_entry);
    new_entry->data = calloc(1, sizeof(KV));
    assert(new_entry->data);
    new_entry->data->key = key;
    new_entry->data->value = value;
    (*hm)->used_size++;

#ifdef NODE_ADD_HAED
    /*头插法*/
    new_entry->next = ((*hm)->maps + index)->next;
    ((*hm)->maps + index)->next = new_entry;
#endif

#ifdef NODE_ADD_TAIL
    /* 尾插*/
    temp->next = new_entry;
    new_entry->next = NULL;
#endif
    return NULL;
}

void *hash_map_get(HashMap *hm, void *key)
{
    assert(hm);
    uint32_t index;
    if (key != NULL)
    {
        size_t i = hash_code(key, hm->hash_algo);
        index = (i & (hm->total_size - 2)) + 1;
    }
    else
    {
        index = 0;
    }
    HashNode *entry;
    for (entry = hm->maps + index; entry != NULL; entry = entry->next)
    {
        if (key == entry->data->key || key_equals(key, entry->data->key))
        {
            return entry->data->value;
        }
    }
    fprintf(stderr, "key not found!\n");
    return NULL;
}
my_bool hash_map_contains(HashMap *hm, const void *key)
{
    assert(hm);
    if (key == NULL)
    {
        return TRUE;
    }
    uint32_t index = (hash_code(key, hm->hash_algo) & (hm->total_size - 2)) + 1;
    HashNode *entry = hm->maps + index;
    if (entry->data->key == NULL)
    {
        return FALSE;
    }
    while (entry)
    {
        if (key_equals(entry->data->key, key))
        {
            return TRUE;
        }
        entry = entry->next;
    }
    return FALSE;
}
void *hash_map_remove(HashMap *hm, void *key)
{
    assert(hm);
    uint32_t index;
    if (key != NULL)
    {
        size_t i = hash_code(key, hm->hash_algo);
        index = (i & (hm->total_size - 2)) + 1;
    }
    else
    {
        index = 0;
    }
    HashNode *entry = hm->maps + index;
    if (entry->data->key == key || key_equals(entry->data->key, key))
    { /*如果首个就是*/
        void *ret = entry->data->value;
        /* free(entry->data);
        entry->data = NULL; */
        /*不应该释放*/
        entry->data->value = NULL;
        entry->data->key = NULL;
        hm->used_size--;
        return ret;
    }

    HashNode *temp;
    while (entry)
    {
        temp = entry;
        entry = entry->next;
        if (entry->data->key == key || key_equals(entry->data->key, key))
        {
            void *ret = entry->data->value;
            temp->next = entry->next;
            free(entry->data);
            entry->data = NULL;
            free(entry);
            hm->used_size--;
            return ret;
        }
    }
    fprintf(stderr, "key not found\n");
    return NULL;
}

void hash_map_print(HashMap *hm, FILE *f, void (*print_key)(FILE *, const void *), void (*print_value)(FILE *, const void *))
{
    fprintf(f, "=============== HashMap at 0x%p ===============\n", hm);
    fprintf(f, "=============== used_size: %10d ===============\n", hm->used_size);
    fprintf(f, "=============== total_size: %9d ===============\n", hm->total_size);
    uint32_t index;
    char *format = long_long_int_format_align_str(hm->total_size);
    for (index = 0; index < hm->total_size; ++index)
    {
        fprintf(f, " ");
        fprintf(f, format, (long long int)index);

        fprintf(f, ": ");
        HashNode *entry = hm->maps + index;
        if (entry->data && entry->data->key)
        {
            while (entry)
            {
                print_key(f, entry->data->key);
                fprintf(f, ": ");
                print_value(f, entry->data->value);
                if (entry->next != NULL)
                {
                    fprintf(f, " -> ");
                }
                entry = entry->next;
            }
            fprintf(f, "\n");
        }
        else
        {
            if (index == 0 && entry->data && entry->data->value)
            {
                fprintf(f, "(null): ");
                print_value(f, entry->data->value);
                fprintf(f, "\n");
            }
            else
            {
                fprintf(f, "[ ]\n");
            }
        }
        if (index == hm->total_size - 1)
        {
            fprintf(f, "================ HashMap Print: Over ================\n");
        }
        else
        {
            fprintf(f, "-----------------------------------------------------\n");
        }
    }
    free(format);
}
