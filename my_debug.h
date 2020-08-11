/*在.h文件中include此文件， 并且再次extern生命alloc和free变量（如果需要的话）
不应该在.c文件中使用该文件*/
#ifndef _MY_DEBUG_H_
#define _MY_DEBUG_H_
/* #undef _MY_DEBUG_H_ */
#ifdef _MY_DEBUG_H_
#include <stdio.h>

#ifndef DEBUG_ALLOC_FREE_COUNT
#define DEBUG_ALLOC_FREE_COUNT
/* #undef DEBUG_ALLOC_FREE_COUNT */
#ifdef DEBUG_ALLOC_FREE_COUNT
#include <stdint.h>
uint64_t g_alloc_count;
uint64_t g_free_count;
#endif
#endif

#ifndef DEBUG_STR_FUNC
#define DEBUG_STR_FUNC
#endif
#undef DEBUG_STR_FUNC

#ifndef DEBUG_VECTOR_PTYPE
#define DEBUG_VECTOR_PTYPE
#endif

#ifndef DEBUG_VECTOR_GENERIC
#define DEBUG_VECTOR_GENERIC
#endif

#ifndef DEBUG_LINKED_LIST_WITHHEAD
#define DEBUG_LINKED_LIST_WITHHEAD
#endif

#ifndef DEBUG_HASH_MAP
#define DEBUG_HASH_MAP
#endif
#undef DEBUG_HASH_MAP

#endif

#endif
