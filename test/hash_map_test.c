#include "hash_map_test.h"
#include "../str_func.h"

#include <time.h>
#include <string.h>

static void print_key_str(FILE *p, const void *key)
{ /*在mingw32当key = null， 会打印一个"(null)"*/
    /*即便如此，也不应该自动处理NULL，不同平台可能不一样。*/
    if (key == NULL)
    {
        fprintf(p, "(null)");
    }
    else
    {
        fprintf(p, "%s", (const char *)key);
    }
}

static void print_value(FILE *p, const void *value)
{ /*当写出这个的时候，说明hash_map的数据仅支持同一类型了*/
    fprintf(p, "%d", *((const int *)value));
}

static int randint_a_b(int a, int b)
{ /*[a,b] random int*/
    if (a > b)
    {
        return 0;
    }
    if (a == b)
    {
        return a;
    }
    int ret = rand() % (b + 1 - a) + a;
    return ret;
}

static my_bool key_str_equals(const void *key1, const void *key2)
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

static size_t key_str_len(const void *key)
{
    return strlen((const char *)key);
}

void hash_map_test_1()
{
#ifdef DEBUG_HASH_MAP
    print_key_str(stderr, NULL);
    fprintf(stdout, "\n");
#endif
    FILE *fp = stderr;
    int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};         /*随便准备了点数据*/
    HashMap *hm = hash_map_new(7, BKDR_hash, key_str_equals, key_str_len); /*其实会创建一个9，因为大于等于7的2的次幂是8，8+1是9*/
    hash_map_print(hm, fp, print_key_str, print_value);
    hash_map_put_kv(&hm, "asdfasdf", &a[0]);
    hash_map_put_kv(&hm, "asdfasd", &a[1]);
    hash_map_put_kv(&hm, "aegacx", &a[2]);
    hash_map_put_kv(&hm, "qwerfe", &a[3]);
    hash_map_put_kv(&hm, "egeawef", &a[4]);
    hash_map_put_kv(&hm, "adsewxwq", &a[5]);
    hash_map_print(hm, fp, print_key_str, print_value);
    hash_map_remove(hm, "adsewxwq");
    hash_map_print(hm, fp, print_key_str, print_value);
    hash_map_put_kv(&hm, "adsewxwq", &a[5]);
    hash_map_put_kv(&hm, "wetwt", &a[6]); /*(6+1)/9>0.75插入这一个应该引发extend*/
    hash_map_print(hm, fp, print_key_str, print_value);
    hash_map_put_kv(&hm, "uipouidjh", &a[7]); /* 再put一个试试*/
    hash_map_print(hm, fp, print_key_str, print_value);
    hash_map_put_kv(&hm, "asdfasd", &a[8]); /*put已有其实是更改*/
    hash_map_print(hm, fp, print_key_str, print_value);
    hash_map_put_kv(&hm, NULL, &a[9]); /*put NULL测试*/
    hash_map_print(hm, fp, print_key_str, print_value);
    const void *ret = hash_map_remove(hm, "qwerfe"); /*remove测试*/
    print_value(fp, ret);
    fprintf(stdout, "\n");
    hash_map_print(hm, fp, print_key_str, print_value);
    ret = hash_map_remove(hm, NULL); /*remove NULL测试*/
    print_value(fp, ret);
    fprintf(stdout, "\n");
    hash_map_print(hm, fp, print_key_str, print_value);
    hash_map_put_kv(&hm, NULL, &a[10]); /*put NULL测试*/
    hash_map_print(hm, fp, print_key_str, print_value);
    hash_map_put_kv(&hm, NULL, NULL); /*put NULL NULL相当于remove null*/
    hash_map_print(hm, fp, print_key_str, print_value);
    hash_map_put_kv(&hm, NULL, &a[11]); /*put NULL测试*/
    hash_map_print(hm, fp, print_key_str, print_value);
    void *test_get = hash_map_get(hm, NULL); /*get NULL*/
    fprintf(stdout, "key=NULL, get value=%d\n", *(int *)test_get);
    fprintf(stdout, "now try to change the value through get func\n");
    *(int *)test_get = 13; /*用get修改*/
    hash_map_print(hm, fp, print_key_str, print_value);
    fprintf(stdout, "now try to change the value of \"egeawef\", using: (*(int *)(hash_map_get(hm, \"egeawef\")))++;\n");
    (*(int *)(hash_map_get(hm, "egeawef")))++;
    hash_map_print(hm, fp, print_key_str, print_value);
    fprintf(stdout, "test get keys, get values\n");
    void **keys = hash_map_keys(hm);
    void **values = hash_map_values(hm);
    size_t i;
    for (i = 1; i < *(size_t *)(*keys + 0); ++i)
    {
        fprintf(stdout, "key %4d: ", i - 1);
        print_key_str(stdout, *(keys + i));
        fprintf(stdout, "\n");
    }
    for (i = 1; i < *(size_t *)(*values + 0); ++i)
    {
        fprintf(stdout, "value %4d: ", i - 1);
        print_value(stdout, *(values + i));
        fprintf(stdout, "\n");
    }
    free(*keys);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    free(*values);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    free(keys);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    free(values);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    hash_map_free(&hm);
}
void hash_map_test_2()
{
    FILE *fp;
    if ((fp = fopen("./test/hash_map_test.txt", "w")) == NULL)
    {
        fprintf(stderr, "Fail to open file!\n");
        return;
    }
#define test_total 3666
    srand((unsigned)time(NULL));
    int total = test_total;
    HashMap *hm = hash_map_new(test_total, BKDR_hash, key_str_equals, key_str_len);
    int i;
    char *s[test_total];
    int a[test_total];
    for (i = 0; i < total; ++i)
    {
        a[i] = i;
        s[i] = my_itoa(randint_a_b(10000, 99999), 10);
#ifdef DEBUG_HASH_MAP
        fprintf(stdout, s[i]);
        fprintf(stdout, "\n");
#endif
    }
    for (i = 0; i < total; ++i)
    {
        hash_map_put_kv(&hm, s[i], &a[i]);
    }
    hash_map_print(hm, fp, print_key_str, print_value);
    hash_map_free(&hm);
    for (i = 0; i < total; ++i)
    {
        free(s[i]);
#ifdef DEBUG_ALLOC_FREE_COUNT
        g_free_count++;
#endif
    }
    fclose(fp);
}

static void print_key_int(FILE *p, const void *key)
{ /*  key 不是int*, key 是 int!   */
    fprintf(p, "%d ", key);
}

static void print_value_int(FILE *p, const void *value)
{
    fprintf(p, "%d ", value);
}

static my_bool key_int_equals(const void *key1, const void *key2)
{ /*  key 不是int*, key 是 int!   */
    return (int)key1 == (int)key2;
}

static size_t key_int_len(const void *key)
{
    return sizeof(int);
}
extern uint32_t BKDR_hash_int(const void *p, size_t len)
{
    int temp = (int)p;
    char *str = (char *)&temp;
    uint32_t seed = 13131; /* 31 131 1313 13131 131313 etc..*/
    uint32_t hash = 0;
    /* while (*str) */
    while (len--)
    {
        hash = hash * seed + (*str++);
    }
    return (hash & 0x7FFFFFFF);
}

void hash_map_test_3()
{
    HashMap *hm_int_int = hash_map_new(7, BKDR_hash_int, key_int_equals, key_int_len);
    hash_map_print(hm_int_int, stdout, print_key_int, print_value_int);

    hash_map_put_kv(&hm_int_int, 1, 3423);
    hash_map_put_kv(&hm_int_int, 2, 3423);
    hash_map_put_kv(&hm_int_int, 6, 3423);
    hash_map_put_kv(&hm_int_int, 5, 3423);
    hash_map_put_kv(&hm_int_int, 3, 5324);
    hash_map_put_kv(&hm_int_int, 4, 6043);
    hash_map_put_kv(&hm_int_int, 0, 1);
    hash_map_put_kv(&hm_int_int, 7, 3423);
    hash_map_print(hm_int_int, stdout, print_key_int, print_value_int);
    hash_map_put_kv(&hm_int_int, 4, (int)(hash_map_get(hm_int_int, 4)) + 1);
    hash_map_print(hm_int_int, stdout, print_key_int, print_value_int);
    hash_map_free(&hm_int_int);

    /* int nums[] = {7, 7, 5, 7, 5, 1, 5, 7, 5, 5, 7, 7, 7, 7, 7, 7} */;
    int nums[] = {-1, 1, 1, 1, 2, 1};
    int numsSize = sizeof(nums) / sizeof(int);
    HashMap *hm = hash_map_new(numsSize, BKDR_hash_int, key_int_equals, key_int_len);
    int i;
    int major = 0, major_count = 0;
    for (i = 0; i < numsSize; ++i)
    {
        hash_map_put_kv(&hm, nums[i], (int)(hash_map_get(hm, nums[i])) + 1);
        if ((int)(hash_map_get(hm, nums[i])) > major_count)
        {
            major = nums[i];
            major_count = (int)(hash_map_get(hm, nums[i]));
        }
    }
    hash_map_print(hm, stdout, print_key_int, print_value_int);
    printf("major is %d\n", major);
    hash_map_free(&hm);
}