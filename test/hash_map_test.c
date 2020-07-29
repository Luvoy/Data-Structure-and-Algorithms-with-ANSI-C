#include "hash_map_test.h"
#include "../str_func.h"
#ifndef HASH_MAP_DEBUG
#define HASH_MAP_DEBUG
#endif
#include <time.h>
void print_key(FILE *p, const void *key)
{ /*当key = null， 会打印一个"(null)"*/
    /*即便如此，也不应该传入NULL，不同平台可能不一样。*/
    fprintf(p, "%s", (const char *)key);
}
void print_value(FILE *p, const void *value)
{ /*当写出这个的时候，说明hash_map的数据仅支持同一类型了*/
    fprintf(p, "%d", *((const int *)value));
}
int randint_a_b(int a, int b)
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
void hash_map_test_1()
{
#ifdef HASH_MAP_DEBUG
    print_key(stderr, NULL);
    fprintf(stdout, "\n");
#endif
    FILE *fp = stderr;
    int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}; /*随便准备了点数据*/
    HashMap *hm = hash_map_new(7, "BKDR");                         /*其实会创建一个9，因为大于等于7的2的次幂是8，8+1是9*/
    hash_map_print(hm, fp, print_key, print_value);
    hash_map_put_kv(&hm, "asdfasdf", &a[0]);
    hash_map_put_kv(&hm, "asdfasd", &a[1]);
    hash_map_put_kv(&hm, "aegacx", &a[2]);
    hash_map_put_kv(&hm, "qwerfe", &a[3]);
    hash_map_put_kv(&hm, "egeawef", &a[4]);
    hash_map_put_kv(&hm, "adsewxwq", &a[5]);
    hash_map_print(hm, fp, print_key, print_value);
    hash_map_remove(hm, "adsewxwq");
    hash_map_print(hm, fp, print_key, print_value);
    hash_map_put_kv(&hm, "adsewxwq", &a[5]);
    hash_map_put_kv(&hm, "wetwt", &a[6]); /*(6+1)/9>0.75插入这一个应该引发extend*/
    hash_map_print(hm, fp, print_key, print_value);
    hash_map_put_kv(&hm, "uipouidjh", &a[7]); /* 再put一个试试*/
    hash_map_print(hm, fp, print_key, print_value);
    hash_map_put_kv(&hm, "asdfasd", &a[8]); /*put已有其实是更改*/
    hash_map_print(hm, fp, print_key, print_value);
    hash_map_put_kv(&hm, NULL, &a[9]); /*put NULL测试*/
    hash_map_print(hm, fp, print_key, print_value);
    const void *ret = hash_map_remove(hm, "qwerfe"); /*remove测试*/
    print_value(fp, ret);
    fprintf(stdout, "\n");
    hash_map_print(hm, fp, print_key, print_value);
    ret = hash_map_remove(hm, NULL); /*remove NULL测试*/
    print_value(fp, ret);
    fprintf(stdout, "\n");
    hash_map_print(hm, fp, print_key, print_value);
    hash_map_put_kv(&hm, NULL, &a[10]); /*put NULL测试*/
    hash_map_print(hm, fp, print_key, print_value);
    hash_map_put_kv(&hm, NULL, NULL); /*put NULL NULL相当于remove null*/
    hash_map_print(hm, fp, print_key, print_value);
    hash_map_put_kv(&hm, NULL, &a[11]); /*put NULL测试*/
    hash_map_print(hm, fp, print_key, print_value);
    void **keys = hash_map_keys(hm);
    void **values = hash_map_values(hm);
    size_t i;
    for (i = 1; i < *(size_t *)(*keys + 0); ++i)
    {
        fprintf(stdout, "key %4d: ", i - 1);
        print_key(stdout, *(keys + i));
        fprintf(stdout, "\n");
    }
    for (i = 1; i < *(size_t *)(*values + 0); ++i)
    {
        fprintf(stdout, "value %4d: ", i - 1);
        print_value(stdout, *(values + i));
        fprintf(stdout, "\n");
    }
    free(*keys);
    free(*values);
    free(*keys);
    free(*values);
    hash_map_free(&hm);
}
void hash_map_test_2()
{
    FILE *fp;
    if ((fp = fopen(".\\test\\hash_map_test.txt", "w")) == NULL)
    {
        fprintf(stderr, "Fail to open file!\n");
        return;
    }
#define test_total 3666
    srand((unsigned)time(NULL));
    int total = test_total;
    HashMap *hm = hash_map_new(test_total, "BKDR");
    int i;
    char *s[test_total];
    int a[test_total];
    for (i = 0; i < total; ++i)
    {
        a[i] = i;
        s[i] = my_itoa(randint_a_b(10000, 99999), 10);
        fprintf(stdout, s[i]);
        fprintf(stdout, "\n");
    }
    for (i = 0; i < total; ++i)
    {
        hash_map_put_kv(&hm, s[i], &a[i]);
    }
    hash_map_print(hm, fp, print_key, print_value);
    hash_map_free(&hm);
    for (i = 0; i < total; ++i)
    {
        free(s[i]);
    }
    fclose(fp);
}
