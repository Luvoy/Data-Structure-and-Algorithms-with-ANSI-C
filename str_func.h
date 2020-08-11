#ifndef _STR_FUNC_H_
#define _STR_FUNC_H_

#include "my_debug.h"
#ifdef DEBUG_ALLOC_FREE_COUNT
extern uint64_t g_alloc_count;
extern uint64_t g_free_count;
#endif
/**
 * 将int数转化成字符串, 可正可负, 进制2-36, 超过返回空指针.
 * 
 * 
 *
*/
char *my_itoa(int num, int radix);
unsigned int get_dig_num(unsigned long long int n);

long long int abs_long_long_int(long long int n);
char *long_long_int_format_align_str(long long int max_len_int);

#endif