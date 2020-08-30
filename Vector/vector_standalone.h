
#include "../my_bool.h"
#include "../my_debug.h"
#ifdef DEBUG_ALLOC_FREE_COUNT
static uint64_t g_alloc_count;
static uint64_t g_free_count;
#endif

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <assert.h>

#ifndef VECTOR_MAX_ALLOCATE_SIZE
#define VECTOR_MAX_ALLOCATE_SIZE 9
#endif

#ifndef VECTOR_NOT_FOUND_INDEX
#define VECTOR_NOT_FOUND_INDEX -1
#endif

#ifndef VECTOR_INDEX_TYPE
#define VECTOR_INDEX_TYPE
typedef int32_t vector_index_type;
#endif

typedef struct Vector
{ /* 这是一种使用宏定义进行泛型的、动态分配内存、内存连续的Vector*/
  vector_index_type allocated_size;
  vector_index_type used_size;
  T *items_p;
} Vector;
static Vector *vector_new(const vector_index_type size);

static Vector *vector_new_from_vector(const Vector *v);
static Vector *vector_new_from_args(vector_index_type size, ...);
static Vector *vector_new_from_elements(vector_index_type size, T *p);

static void vector_print(const Vector *v, FILE *fp, const char *format, void (*elem_print)(FILE *, const T));

static void vector_free(Vector **v);

static vector_index_type vector_length(const Vector *v);
static T vector_element_at(const Vector *v, vector_index_type index, my_bool allow_negative_index);
static void vector_index_assign(const Vector *v, vector_index_type index, T elem);
static vector_index_type vector_left_search(const Vector *v, T key, my_bool (*elem_equal)(const T, const T));
static void vector_append(Vector *v, T elem);
static void vector_join(Vector *v_left, Vector *v_right);
static T vector_pop(Vector *v);
static T vector_pop_i(Vector *v, vector_index_type index);
static void vector_insert(Vector *v, vector_index_type index, T elem);
static Vector *vector_slice(Vector *v, vector_index_type start, vector_index_type end, vector_index_type step);
static void vector_reverse(Vector *v);

static T *vector_calloc_data(vector_index_type size)
{ /* 开辟序列的内存空间，相当于为每个元素的指针开辟空间，但是并不为每个元素开辟空间*/
  T *temp = (T *)calloc(size, sizeof(T));
#ifdef DEBUG_ALLOC_FREE_COUNT
  g_alloc_count++;
#endif
  assert(temp);
  return temp;
}

static void vector_free_data(Vector *v)
{ /*释放序列的内存空间， 包括释放每个元素的内存空间*/
  if (v == NULL)
  {
    return;
  }
  if (v->items_p)
  {
    free(v->items_p);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    v->items_p = NULL;
  }
}

static Vector *vector_new(const vector_index_type size)
{
  Vector *v;
  assert(size > 0);
  v = (Vector *)calloc(1, sizeof(Vector));
#ifdef DEBUG_ALLOC_FREE_COUNT
  g_alloc_count++;
#endif
  assert(v);
  v->allocated_size = ((size / VECTOR_MAX_ALLOCATE_SIZE) + 1) * VECTOR_MAX_ALLOCATE_SIZE;
  v->used_size = size;
  v->items_p = vector_calloc_data(v->allocated_size);
  return v;
}

static Vector *vector_new_from_vector(const Vector *v)
{ /*浅拷贝*/
  Vector *v_new = vector_new(v->used_size);
#ifdef DEBUG_VECTOR
  fprintf(stderr, "In [vector_new_from_vector] new vector: OK\n");
#endif
  vector_index_type i;
  for (i = 0; i < v->used_size; ++i)
  {
    *(v_new->items_p + i) = *(v->items_p + i);
  }
  return v_new;
}

static Vector *vector_new_from_args(vector_index_type size, ...)
{
  assert(size > 0);
  Vector *v_new = vector_new(size);
  assert(v_new != NULL);
#ifdef DEBUG_VECTOR
  fprintf(stderr, "In [vector_new_from_args] new vector: OK\n");
#endif
  va_list vl;
  va_start(vl, size);
  vector_index_type i;
  for (i = 0; i < size; ++i)
  {
    *(v_new->items_p + i) = va_arg(vl, T);
  }
  va_end(vl);
  return v_new;
}

static Vector *vector_new_from_elements(vector_index_type size, T *p)
{
  Vector *v = vector_new(size);
  assert(v != NULL);
#ifdef DEBUG_VECTOR
  fprintf(stderr, "In [vector_new_from_elements] new vector: OK\n");
#endif
  vector_index_type i;
  for (i = 0; i < size; ++i)
  {
    *(v->items_p + i) = *(p + i);
  }
  return v;
}

static void vector_print(const Vector *v, FILE *fp, const char *format, void (*elem_print)(FILE *, const T))
{
  if (v == NULL)
  {
    fprintf(fp, "NULL vector!\n");
    return;
  }
  vector_index_type i;
  if (strcmp(format, "SINGLE_LINE") == 0)
  {
    for (i = 0; i < v->used_size; ++i)
    {
      if (i == 0)
      {
        fprintf(fp, "[");
      }
      elem_print(fp, *((v->items_p) + i));
      if (i != v->used_size - 1)
      {

        fprintf(fp, ", ");
      }
      else
      {
        fprintf(fp, "]\n");
      }
    }
  }
  else if (strcmp(format, "MULTI_LINES") == 0)
  {
    for (i = 0; i < v->used_size; ++i)
    {
      elem_print(fp, *((v->items_p) + i));
      fprintf(fp, "\n");
    }
  }
  else
  {
    fprintf(fp, "Undefined format string: %s\n", format);
  }
  return;
}

static void vector_free(Vector **v)
{
  assert(v);
  if (*v == NULL)
  {
    return;
  }
  vector_free_data(*v);
#ifdef DEBUG_VECTOR
  fprintf(stderr, "In [vector_free] data: OK\n");
#endif
  free(*v);
#ifdef DEBUG_ALLOC_FREE_COUNT
  g_free_count++;
#endif
  *v = NULL;
}

static vector_index_type vector_length(const Vector *v)
{
  assert(v != NULL);
  return v->used_size;
}

static T vector_element_at(const Vector *v, vector_index_type index, my_bool allow_negative_index)
{
  assert(v != NULL);
  if (!allow_negative_index)
  {
    if (index < 0 || index >= v->used_size)
    {
      fprintf(stderr, "Illegal index!\n");
      return NULL;
    }
    return *((v->items_p) + index);
  }
  else
  {
    if (index < -(v->used_size) || index >= v->used_size)
    {
      fprintf(stderr, "Illegal index!\n");
      return NULL;
    }
    vector_index_type real_index = index > 0 ? index : v->used_size + index;
    return *((v->items_p) + real_index);
  }
}

static void vector_index_assign(const Vector *v, vector_index_type index, T elem)
{
  assert(v != NULL);
  if (index < 0 || index >= v->used_size)
  {
    fprintf(stderr, "Illegal index!\n");
    return;
  }
  *((v->items_p) + index) = elem;
  return;
}

static vector_index_type vector_left_search(const Vector *v, T key, my_bool (*elem_equal)(const T, const T))
{
  assert(v != NULL);
  vector_index_type i;
  for (i = 0; i < v->used_size; ++i)
  {
    if (elem_equal(key, *((v->items_p) + i)) == TRUE)
    {
      return i;
    }
  }
  return VECTOR_NOT_FOUND_INDEX;
}

static void vector_append(Vector *v, T elem)
{
  assert(v != NULL);
  if (v->used_size < v->allocated_size)
  {
    *((v->items_p) + (v->used_size)) = elem;
    v->used_size++;
  }
  else
  {
    T *temp = vector_calloc_data(v->allocated_size + VECTOR_MAX_ALLOCATE_SIZE);
    assert(temp);
    vector_index_type i;
    for (i = 0; i < v->used_size; ++i)
    {
      *(temp + i) = *((v->items_p) + i);
    }
    *(temp + i) = elem;
    vector_free_data(v);
    v->used_size++;
    v->items_p = temp;
  }
}

static void vector_join(Vector *v_left, Vector *v_right)
{ /*浅拷贝.连接之后， 当right删除、添加元素的时候，left不变. */
  assert(v_left != NULL);
  assert(v_right != NULL);

  if (v_left->used_size + v_right->used_size <= v_left->allocated_size)
  {
    vector_index_type i;
    for (i = 0; i < v_right->used_size; ++i)
    { /*把right的元素的地址拿过来*/
      *((v_left->items_p) + (v_left->used_size) + i) = *((v_right->items_p) + i);
    }
    v_left->used_size += v_right->used_size;

    /* vector_free(v_right); */
    /* 不应该free right */
  }
  else
  {
    T *temp = vector_calloc_data(VECTOR_MAX_ALLOCATE_SIZE * (((v_left->used_size) + (v_right->used_size)) / VECTOR_MAX_ALLOCATE_SIZE + 1));
    assert(temp);
    vector_index_type i;
    for (i = 0; i < v_left->used_size; ++i)
    {
      *(temp + i) = *((v_left->items_p) + i);
    }
    for (i = 0; i < v_right->used_size; ++i)
    {
      *(temp + (v_left->used_size) + i) = *((v_right->items_p) + i);
    }
    v_left->used_size += v_right->used_size;

    vector_free_data(v_left);
    v_left->items_p = temp;

    /* vector_free(v_right); */
    /* 不应该free right */
  }
}

static T vector_pop(Vector *v)
{
  assert(v != NULL);
  if (v->used_size < 1)
  {
    fprintf(stderr, "No element to pop!\n");
    return NULL;
  }

  T temp = *((v->items_p) + v->used_size - 1);
  *((v->items_p) + v->used_size - 1) = NULL;
  v->used_size--;
  return temp;
}

static T vector_pop_i(Vector *v, vector_index_type index)
{
  assert(v != NULL);
  if (index < 0 || index > v->used_size - 1)
  {
    fprintf(stderr, "Illegal index!\n");
    return NULL;
  }
  vector_index_type i;
  T ret = *((v->items_p) + index);
  for (i = index; i < v->used_size - 1; ++i)
  {
    *((v->items_p) + i) = *((v->items_p) + i + 1);
  }
  *((v->items_p) + i) = NULL;
  v->used_size--;
  return ret;
}

static void vector_insert(Vector *v, vector_index_type index, T elem)
{
  assert(v != NULL);
  if (index < 0 || index > v->used_size)
  {
    fprintf(stderr, "Illegal index!\n");
    return;
  }
  if (index == v->used_size)
  {
    vector_append(v, elem);
    return;
  }
  else
  {
    vector_index_type i;
    if (v->used_size == v->allocated_size)
    {
      T *temp = vector_calloc_data(v->allocated_size + VECTOR_MAX_ALLOCATE_SIZE);
      assert(temp);
      for (i = 0; i < index; ++i)
      {
        *(temp + i) = *((v->items_p) + i);
      }
      *(temp + i) = elem;
      v->used_size++;
      for (++i; i < v->used_size; ++i)
      {
        *(temp + i) = *((v->items_p) + i - 1);
      }
      vector_free_data(v);
      v->items_p = temp;
    }
    else
    {
      for (i = v->used_size; i > index; --i)
      {
        *((v->items_p) + i) = *((v->items_p) + i - 1);
      }
      *((v->items_p) + i) = elem;
      v->used_size++;
    }
  }
}

static Vector *vector_slice(Vector *v, vector_index_type start, vector_index_type end, vector_index_type step)
{                    /*切片， 支持负索引。类似于python。调用者必须free*/
  assert(v != NULL); /* 检查传入指针合法性 */
  if (step == 0)
  { /* 检查 step 不为0 */
    fprintf(stderr, "Illegal step!\n");
    return NULL;
  }
  if (start < -(v->used_size + 1) || start > (v->used_size) || end < -(v->used_size + 1) || end > (v->used_size))
  { /* 检查 索引越界 */
    fprintf(stderr, "Illegal index!\n");
    return NULL;
  }

  /* 将正负索引统一为正索引*/
  vector_index_type positv_start = start >= 0 ? start : v->used_size + start;
  vector_index_type positv_end = end >= 0 ? end : v->used_size + end;

  /* 索引先后和步长正负判断 */
  if ((positv_start < positv_end && step < 0) || (positv_start > positv_end && step > 0))
  {
    fprintf(stderr, "Step dose not match index's direction!\n");
    return NULL;
  }

  /* 求切片后的长度 */
  vector_index_type new_length = abs(positv_end - positv_start) % abs(step) == 0 ? abs(positv_end - positv_start) / abs(step) : (positv_end - positv_start) / abs(step) + 1;
  if (new_length == 0)
  {
    fprintf(stderr, "Zero length!\n");
    return NULL;
  }

  /* 分配新vector */
  Vector *v_new = vector_new(new_length);
  assert(v_new != NULL);

  /* 赋值 */
  vector_index_type i = 0;
  vector_index_type j = 0;
  for (; i < new_length; i++)
  {
    *((v_new->items_p) + i) = *((v->items_p) + j + positv_start);
    j += step;
  }
  return v_new;
}

static void vector_reverse(Vector *v)
{
  /*注意 *(items_p+i)是连续的, *(items_p+i)中的值不一定是连续的*/
  uint8_t *head = (uint8_t *)(v->items_p + 0);
  uint8_t *tail = (uint8_t *)(v->items_p + v->used_size - 1);
  uint8_t *temp = (uint8_t *)calloc(sizeof(T), sizeof(uint8_t));
#ifdef DEBUG_ALLOC_FREE_COUNT
  g_alloc_count++;
#endif
  while (head < tail)
  {

    memcpy(temp, head, sizeof(T));
    memcpy(head, tail, sizeof(T));
    memcpy(tail, temp, sizeof(T));
    head += sizeof(T);
    tail -= sizeof(T);
  }
  free(temp);
#ifdef DEBUG_ALLOC_FREE_COUNT
  g_free_count++;
#endif
  temp = NULL;
}

#undef T