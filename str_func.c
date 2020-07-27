#include <stdlib.h>
#include <string.h>
#include <assert.h>
char *my_itoa(int num, int radix)
{
    if (radix < 2 || radix > 36)
    {
        return (char *)0;
    }
    char index[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int i;
    int str_len = 1;
    unsigned int unum;
    unsigned int unum_copy;
    if (num < 0)
    {
        unum = (unsigned int)-num;
        str_len++;
    }
    else
    {
        unum = (unsigned int)num;
    }
    unum_copy = unum;
    do
    {
        unum /= radix;
        str_len++;
    } while (unum);
    char *ret = (char *)malloc(str_len * sizeof(char));

    ret[str_len - 1] = '\0';
    i = str_len - 2;
    do
    {
        ret[i--] = index[unum_copy % (unsigned int)radix];
        unum_copy /= radix;
    } while (unum_copy);
    if (num < 0)
    {
        ret[i] = '-';
    }
    return ret;
}
unsigned int get_dig_num(unsigned long long int n)
{
    if (!n)
    {
        return 1;
    }
    unsigned int ret = 0;
#ifdef STR_FUNC_DEBUG
    unsigned long long int temp = n;
#endif
    while (n)
    {
        n /= 10;
        ret++;
    }
#ifdef STR_FUNC_DEBUG
    fprintf(stderr, "get_dig_num(%llu) is %u\n", temp, ret);
#endif
    return ret;
}

long long int abs_long_long_int(long long int n)
{
    long long int ret;
    if (n >= 0)
    {
        ret = n;
    }
    else
    {
        ret = -1 * n;
    }
#ifdef STR_FUNC_DEBUG
    fprintf(stderr, "abs %lld is %lld\n", n, ret);
#endif
    return ret;
}
char *long_long_int_format_align_str(long long int max_len_int)
{ /* 输出多行数字时能对齐，实现类似如下效果：
  *   1 ... 
  *   2 ... 
  *   3 ... 
  * ... ...
  *  58 ...
  * ... ...
  * 999 ...
  */
    unsigned int unsigned_dig_num = get_dig_num(abs_long_long_int(max_len_int));
    if (max_len_int < 0)
    {
        unsigned_dig_num++;
    }
    char *s = (char *)calloc(get_dig_num(unsigned_dig_num) + 5, sizeof(char));
    /* % l l d '\0' 共5个*/
    assert(s);
    strcat(s, "%");
    char *temp = my_itoa(unsigned_dig_num, 10);
#ifdef STR_FUNC_DEBUG
    fprintf(stderr, "my_itoa(%d) is %s\n", unsigned_dig_num, temp);
#endif
    strcat(s, temp);
    free(temp);
    strcat(s, "lld");
#ifdef STR_FUNC_DEBUG
    printf("ret is %s\n", s);
#endif
    return s;
}