#include "../my_bool.h"
#include "../my_debug.h"
#ifdef DEBUG_ALLOC_FREE_COUNT
static uint64_t g_alloc_count;
static uint64_t g_free_count;
#endif
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <stddef.h>

#ifndef _HASH_MAP_SIZE_TYPE_
#define _HASH_MAP_SIZE_TYPE_
typedef int32_t hash_map_size_type; /* 负值是有需要的 */
#endif

#ifndef _KEY_SIZE_TYPE_
#define _KEY_SIZE_TYPE_
typedef uint32_t key_size_type;
#endif

#ifndef _HASH_CODE_TYPE_
#define _HASH_CODE_TYPE_
typedef uint32_t hash_code_type;
#endif

typedef struct KV
{
    K key;
    V value;
} KV;

typedef struct HashNode
{
    struct HashNode *next;
    KV *data;
} HashNode;

typedef struct HashMap
{
    hash_map_size_type total_size;
    hash_map_size_type used_size;
    /* const char *hash_algo; */
    hash_code_type (*p_func_hash_code)(const K, key_size_type);
    my_bool (*p_func_key_equal)(const K, const K);
    key_size_type (*p_func_key_len)(const K);
    my_bool (*p_func_value_equal)(const V, const V);
    HashNode *maps;
} HashMap;

static hash_code_type BKDR_hash(const K key, key_size_type len);
static HashMap *hash_map_new(hash_map_size_type n, hash_code_type (*p_func_hash_code)(const K, key_size_type), my_bool (*p_func_key_equal)(const K, const K), key_size_type (*p_func_key_len)(const K), my_bool (*p_func_value_equal)(const V, const V));
static void hash_map_free(HashMap **hm);
static HashMap *hash_map_extend(HashMap **hm);
static V hash_map_put_kv(HashMap **hm, const K key, const V value);

static V hash_map_get(HashMap *hm, K key);

static my_bool hash_map_contains(HashMap *hm, const K key);
static V hash_map_remove(HashMap *hm, K key);
static K *hash_map_keys(HashMap *hm, hash_map_size_type *keys_count);
static V *hash_map_values(HashMap *hm, hash_map_size_type *values_count);
static void hash_map_print(HashMap *hm, FILE *f, void (*print_key)(FILE *, const K), void (*print_value)(FILE *, const V));

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

static hash_code_type BKDR_hash(const K key, key_size_type len)
{ /* example */
    char *str = (char *)key;
    hash_code_type seed = 131; /* 31 131 1313 13131 131313 etc..*/
    hash_code_type hash = 0;
    /* while (*str) */
    while (len--)
    {
        hash = hash * seed + (*str++);
    }
    return (hash & 0x7FFFFFFF);
}

static HashMap *hash_map_new(hash_map_size_type n, hash_code_type (*p_func_hash_code)(const K, key_size_type), my_bool (*p_func_key_equal)(const K, const K), key_size_type (*p_func_key_len)(const K), my_bool (*p_func_value_equal)(const V, const V))
{
    if (n < 0)
    {
        fprintf(stderr, "size is negative!");
    }
    hash_map_size_type temp = 1;
    while (temp < n)
    {
        temp <<= 1;
    } /* 让hashmap的总容量为2的次幂*/

    HashMap *hm = (HashMap *)calloc(1, sizeof(HashMap));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    assert(hm);
    hm->total_size = temp + 1; /* 给null留一个*/
    hm->used_size = 0;
    hm->p_func_hash_code = p_func_hash_code;
    hm->p_func_key_equal = p_func_key_equal;
    hm->p_func_key_len = p_func_key_len;
    hm->p_func_value_equal = p_func_value_equal;
    hm->maps = (HashNode *)calloc(hm->total_size, sizeof(HashNode));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    assert(hm->maps);
    hash_map_size_type i;
    for (i = 0; i < hm->total_size; ++i)
    {
        (hm->maps + i)->next = NULL;
        (hm->maps + i)->data = (KV *)calloc(1, sizeof(KV));
#ifdef DEBUG_ALLOC_FREE_COUNT
        g_alloc_count++;
#endif
        assert((hm->maps + i)->data);
    }
    return hm;
}

static void hash_map_free(HashMap **hm)
{
    hash_map_size_type i;
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
#ifdef DEBUG_ALLOC_FREE_COUNT
                g_free_count++;
#endif
                p_second->data = NULL;
                free(p_second);
#ifdef DEBUG_ALLOC_FREE_COUNT
                g_free_count++;
#endif
            }
        }
        p_second = NULL;
        if (p_last != (*hm)->maps + i)
        { /*如果最后一个节点不是第一个（也就是尾指针移动了）*/
            /*释放尾*/
            free(p_last->data);
#ifdef DEBUG_ALLOC_FREE_COUNT
            g_free_count++;
#endif
            p_last->data = NULL;
            free(p_last);
#ifdef DEBUG_ALLOC_FREE_COUNT
            g_free_count++;
#endif
            p_last = NULL;
        }

        /*释放头*/
        free(((*hm)->maps + i)->data);
#ifdef DEBUG_ALLOC_FREE_COUNT
        g_free_count++;
#endif
        ((*hm)->maps + i)->data = NULL;
    }
    free((*hm)->maps);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    (*hm)->maps = NULL;
    free(*hm);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    *hm = NULL;
}

static HashMap *hash_map_extend(HashMap **hm)
{ /* 重新创建一个hash_map, 复制内容, 释放原来的, 返回新的*/

    HashMap *hm_new = hash_map_new((*hm)->total_size, (*hm)->p_func_hash_code, (*hm)->p_func_key_equal, (*hm)->p_func_key_len, (*hm)->p_func_value_equal);
    /*这里直接传入hm的总size即可，因为size一直是2的次幂+1，会自动*2并且+1*/
    HashNode *entry;
    hash_map_size_type hash_index;
    for (hash_index = 0; hash_index < (*hm)->total_size; hash_index++)
    {
        for (entry = (*hm)->maps + hash_index; entry != NULL; entry = entry->next)
        {
            hash_map_put_kv(&hm_new, entry->data->key, entry->data->value);
        }
    }
    hash_map_free(hm);
    return hm_new;
}

static V hash_map_put_kv(HashMap **hm, const K key, const V value)
{
    assert(hm);
    assert((*hm));
    V ret = V_NULL;

    if ((double)((*hm)->used_size + 1) / (double)((*hm)->total_size) > 0.75)
    {
        (*hm) = hash_map_extend(hm);
    }
    hash_map_size_type index;
    if ((*hm)->p_func_key_equal(key, K_NULL) == FALSE)
    { /* key 不是NULL */
        hash_code_type hash = (*hm)->p_func_hash_code(key, (*hm)->p_func_key_len(key));
        index = (hash & ((*hm)->total_size - 2)) + 1;
#ifdef DEBUG_HASH_MAP
        fprintf(stdout, "hash of key: %u, index: %u\n", hash, index);
#endif
    }
    else
    { /* key is null */
        index = 0;
        ret = ((*hm)->maps + index)->data->value;
        if ((*hm)->p_func_value_equal(ret, V_NULL) == TRUE)
        { /* ret value is  null */
            if ((*hm)->p_func_value_equal(value, V_NULL) == TRUE)
            { /* put value is null */
                return V_NULL;
            }
            /* put value is not null */
            ((*hm)->maps + index)->data->key = K_NULL; /* 是不是data还是应该定义成K* V* ，赋值的时候用memcpy好一些 */
            ((*hm)->maps + index)->data->value = value;
            (*hm)->used_size++;
            return ret;
        }
        else
        { /*ret value is not null */
            if ((*hm)->p_func_value_equal(value, V_NULL) == TRUE)
            { /*put value is null */
                (*hm)->used_size--;
            }
            ((*hm)->maps + index)->data->value = value;
            return ret;
        }
    }
    /*key is not null */
    HashNode *entry;
    if ((*hm)->p_func_key_equal(((*hm)->maps + index)->data->key, K_NULL))
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
        if (entry->data->key == key || (*hm)->p_func_key_equal(entry->data->key, key))
        { /*已有 ,直接覆盖*/
            ret = entry->data->value;
            entry->data->value = value;
            return ret;
        }
    }

    HashNode *new_entry = (HashNode *)calloc(1, sizeof(HashNode));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    assert(new_entry);
    new_entry->data = calloc(1, sizeof(KV));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
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
    return V_NULL;
}

static V hash_map_get(HashMap *hm, K key)
{
    assert(hm);
    hash_map_size_type index;
    if (hm->p_func_key_equal(key, K_NULL) == FALSE)
    {
        hash_code_type i = hm->p_func_hash_code(key, hm->p_func_key_len(key));
        index = (i & (hm->total_size - 2)) + 1;
    }
    else
    {
        index = 0;
    }
    HashNode *entry;
    for (entry = hm->maps + index; entry != NULL; entry = entry->next)
    {
        if (key == entry->data->key || hm->p_func_key_equal(key, entry->data->key))
        {
            return entry->data->value;
        }
    }
    fprintf(stderr, "key not found!\n");
    return V_NULL;
}

my_bool hash_map_contains(HashMap *hm, const K key)
{
    assert(hm);
    if (hm->p_func_key_equal(key, K_NULL) == TRUE)
    {
        return TRUE;
    }
    hash_map_size_type index = (hm->p_func_hash_code(key, hm->p_func_key_len(key)) & (hm->total_size - 2)) + 1;
    HashNode *entry = hm->maps + index;
    if (hm->p_func_key_equal(entry->data->key, K_NULL) == TRUE)
    {
        return FALSE;
    }
    while (entry)
    {
        if (hm->p_func_key_equal(entry->data->key, key))
        {
            return TRUE;
        }
        entry = entry->next;
    }
    return FALSE;
}

V hash_map_remove(HashMap *hm, K key)
{
    assert(hm);
    hash_map_size_type index;
    if (hm->p_func_key_equal(key, K_NULL) == FALSE)
    {
        hash_code_type i = hm->p_func_hash_code(key, hm->p_func_key_len(key));
        index = (i & (hm->total_size - 2)) + 1;
    }
    else
    {
        index = 0;
    }
    HashNode *entry = hm->maps + index;
    if (entry->data->key == key || hm->p_func_key_equal(entry->data->key, key))
    { /*如果首个就是*/
        V ret = entry->data->value;
        /* free(entry->data);
#ifdef DEBUG_ALLOC_FREE_COUNT
            g_free_count++;
#endif
        entry->data = NULL; */
        /*不应该释放*/
        entry->data->value = V_NULL;
        entry->data->key = K_NULL;
        hm->used_size--;
        return ret;
    }

    HashNode *temp;
    while (entry)
    {
        temp = entry;
        entry = entry->next;
        if (hm->p_func_key_equal(entry->data->key, key))
        {
            V ret = entry->data->value;
            temp->next = entry->next;
            free(entry->data);
#ifdef DEBUG_ALLOC_FREE_COUNT
            g_free_count++;
#endif
            entry->data = NULL;
            free(entry);
#ifdef DEBUG_ALLOC_FREE_COUNT
            g_free_count++;
#endif
            hm->used_size--;
            return ret;
        }
    }
    fprintf(stderr, "key not found\n");
    return V_NULL;
}

K *hash_map_keys(HashMap *hm, hash_map_size_type *keys_count)
{ /* 开辟了used_size个K, caller must free(return)*/
    assert(hm);
    K *keys = calloc(hm->used_size, sizeof(K));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    assert(keys);

    HashNode *entry;
    hash_map_size_type hash_index;
    hash_map_size_type used_index = 0;
    for (hash_index = 0; hash_index < hm->total_size; hash_index++)
    {
        for (entry = hm->maps + hash_index; entry != NULL; entry = entry->next)
        {

            if ((hash_index == 0 && entry->data && entry->data->value) || (hash_index != 0 && entry->data && entry->data->key))
            {
                *(keys + used_index) = entry->data->key;
                used_index++;
            }
        }
    }
    *keys_count = hm->used_size;
    return keys;
}

V *hash_map_values(HashMap *hm, hash_map_size_type *values_count)
{ /* 开辟了used_size 个V, 调用者必须free(return)*/
    assert(hm);
    V *values = calloc(hm->used_size, sizeof(V));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    assert(values);

    HashNode *entry;
    hash_map_size_type hash_index;
    hash_map_size_type used_index = 0;
    for (hash_index = 0; hash_index < hm->total_size; hash_index++)
    {
        for (entry = hm->maps + hash_index; entry != NULL; entry = entry->next)
        {

            if ((hash_index == 0 && entry->data && entry->data->value) || (hash_index != 0 && entry->data && entry->data->key))
            {
                *(values + used_index) = entry->data->value;
                used_index++;
            }
        }
    }
    *values_count = hm->used_size;
    return values;
}

void hash_map_print(HashMap *hm, FILE *f, void (*print_key)(FILE *, const K), void (*print_value)(FILE *, const V))
{
    fprintf(f, "=================== HashMap at %18p ===================\n", hm);
    fprintf(f, "================== used_size: %20llu ==================\n", (hm->used_size));
    fprintf(f, "================= total_size: %20llu ==================\n", (hm->total_size));
    hash_map_size_type index;
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
            fprintf(f, "======================== HashMap Print: Over ========================\n");
        }
        else
        {
            fprintf(f, "---------------------------------------------------------------------\n");
        }
    }
    free(format);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
}

#undef K
#undef V