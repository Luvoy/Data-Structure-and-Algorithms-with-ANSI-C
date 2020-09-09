#include <stdlib.h>
#include <stdint.h>
// restrict the lengths of all types to avoid variety of length using different compilers
#include <inttypes.h>
// avoid mismatch between format str and type length in cross-platform environment
#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include <math.h>

#define DEBUG_RBTREE
#define DEBUG_ALLOC_FREE_COUNT
uint64_t g_alloc_count = 0;
uint64_t g_free_count = 0;

typedef int64_t cmp_ret_type;
typedef uint8_t status;
#define STAUTS_NOK ((status)0)
#define STATUS_OK ((status)1)
#define STATUS_NEED_FREE ((status)2)
typedef uint8_t my_bool;
#define TRUE ((my_bool)1)
#define FALSE ((my_bool)0)

// 注释以下两行中的一行来使用treemap或者treeset
#define MY_TREE_MAP
#define MY_TREE_SET
// 注释以上两行中的一行来使用treemap或者treeset

#ifdef MY_TREE_MAP
#undef MY_TREE_SET
#endif

#ifdef MY_TREE_SET
#undef MY_TREE_MAP
#endif

/***************************** TREE_MAP *****************************/
#ifdef MY_TREE_MAP
#define K int32_t     //replace it with what you want
#define K_NULL ((K)0) //replace it with what you want
#define V char *      //replace it with what you want
#define V_NULL NULL   //replace it with what you want

typedef struct KV
{
    K key;
    V value;
} KV;

#define T KV
//#define T KV *      //also you can try struct pointer instead of struct
#define T_NULL ((KV){K_NULL, V_NULL})
#include <stdio.h>
static void elem_print_imp(FILE *fp, const T elem)
{ //implement: void(*)(FILE*,const T) //replace it with what you want
    if (elem.value == NULL)

        fprintf(fp, "%" PRId32 ":(null)", elem.key);
    else
        fprintf(fp, "%" PRId32 ":%s", elem.key, elem.value);
}
static cmp_ret_type elem_compare(const T a, const T b)
{ //implement: cmp_ret_type(*)(const T, const T) //replace it with what you want
    return a.key - b.key;
}
#endif
/***************************** TREE_MAP *****************************/

/***************************** TREE_SET *****************************/
#ifdef MY_TREE_SET
#define T int32_t     //replace it with what you want
#define T_NULL ((T)0) //replace it with what you want
#include <stdio.h>
static void elem_print(FILE *fp, const T elem)
{ //implement: void(*)(FILE*,const T) //replace it with what you want
    fprintf(fp, "%" PRId32 "", elem);
}

static cmp_ret_type elem_compare(const T a, const T b)
{ //implement: cmp_ret_type(*)(const T, const T) //replace it with what you want
    return a - b;
}

#endif
/***************************** TREE_SET *****************************/
#if defined(MY_TREE_MAP) || (MY_TREE_SET)

typedef uint8_t color_type;
#define RED ((color_type)0) //当节点被calloc创建，全0，即默认红色
#define BLACK ((color_type)1)

typedef uint8_t ptr_tag;
#define THREAD ((ptr_tag)0)
#define LINK ((ptr_tag)1)

typedef uint64_t rbtree_size_type;

typedef struct RBNode
{ //节点
    T data;
    struct RBNode *left;
    struct RBNode *right;
    struct RBNode *parent;
    color_type color;
} RBNode;

typedef struct RBTree
{ //红黑树的驱动结构体
    RBNode *root;
    rbtree_size_type node_count;
} RBTree;

static rbtree_size_type my_log_floor(const rbtree_size_type n);
//返回>=log2(n)的整数

static void rbnode_set_value(RBNode *rbn_p, const T elem);
//更新节点中的data，当T为键值对时，可以用于仅更新value

static color_type rbnode_get_color(const RBNode *rbn_p);
//获得节点颜色

static void rbnode_set_color(RBNode *rbn_p, color_type new_color);
//设置节点颜色

static RBNode *rbnode_new(const T elem);
//建立非空节点

static RBTree *rbtree_new_empty(void);
//创建一个根为NULL的红黑树，只有驱动

static RBTree *rbtree_new_from_array(const T *elem_arr, const rbtree_size_type num, cmp_ret_type (*elem_cmp)(const T, const T));
//由数组创建红黑树

static rbtree_size_type rbtree_elem_count_quick(const RBTree *rbt_p);
//元素个数快速版

static RBNode *rbtree_search(const RBTree *rbt_p, const T elem, cmp_ret_type (*elem_cmp)(const T, const T));
//查找某元素所在的节点

static void rbtree_left_rotate(RBTree *rbt_p, RBNode *pivot);
//左旋

static void rbtree_right_rotate(RBTree *rbt_p, RBNode *pivot);
//右旋

static void rbtree_insert_elem(RBTree *rbt_p, const T elem, cmp_ret_type (*elem_cmp)(const T, const T));
//插入元素

static status rbtree_insert_node(RBTree *rbt_p, RBNode *rbn_p, cmp_ret_type (*elem_cmp)(const T, const T));
//插入节点

static void rbtree_insert_fixup(RBTree *rbt_p, RBNode *rbn_p);
//插入节点后的平衡修复

static void rbtree_delete_elem(RBTree *rbt_p, T elem, cmp_ret_type (*elem_cmp)(const T, const T));
//删除元素

static void rbtree_delete_node(RBTree *rbt_p, RBNode *node);
//删除节点

static void rbtree_delete_fixup(RBTree *rbt_p, RBNode *node, RBNode *parent);
//删除节点后的平衡修复

static RBNode *rbtree_first(RBTree *rbt_p);
//取得中序遍历时的第一个节点，也就是最左的节点

static RBNode *rbtree_last(RBTree *rbt_p);
//取得中序遍历时的最后一个节点，也就是最右的节点

static RBNode *rbtree_next(RBNode *node);
//根据当前节点，取得中序遍历时的下一个节点，不符合条件可能会返回NULL

static RBNode *rbtree_prev(RBNode *node);
//根据当前节点，取得中序遍历时的下一个节点，不符合条件可能会返回NULL

static void rbtree_destroy(RBTree **rbt_pp);
//销毁红黑树

static void rbtree_preorder_traverse(const RBTree *rbt_p, status (*visit)(RBNode *));
//前序遍历

static void rbtree_inorder_traverse(const RBTree *rbt_p, status (*visit)(RBNode *));
//中序遍历

static void rbtree_postorder_traverse(const RBTree *rbt_p, status (*visit)(RBNode *));
//后序遍历

static void rbtree_visual_print(FILE *fp, const RBTree *rbt_p, RBNode *node, rbtree_size_type high, rbtree_size_type n, void (*elem_print)(FILE *, const T));
//可视化输出

static rbtree_size_type rbtree_elem_count(const RBTree *rbt_p, my_bool use_my_stack);
//元素个数遍历版本

/***********************************************************************/

#define STACK_T RBNode *
#define STACK_T_NULL NULL

typedef int64_t stack_size_type;

typedef struct Stack
{
    STACK_T *base; //
    STACK_T *top;  //指元素尾部, 而不是指向元素起始地址处
    stack_size_type size;
} Stack;

cmp_ret_type stack_cmp(const STACK_T a, const STACK_T b)
{
    return a - b;
}

static my_bool stack_is_empty(const Stack *s)
{
    if (s->base >= s->top)
    {
        return TRUE;
    }
    return FALSE;
}

static my_bool stack_is_full(const Stack *s)
{
    if (s->base + (s->size) == s->top)
    {
        return TRUE;
    }
    return FALSE;
}

static stack_size_type stack_used_size(const Stack *s)
{
    assert(s != NULL);
    return s->top - s->base;
}

static stack_size_type stack_total_size(const Stack *s)
{
    assert(s != NULL);
    return s->size;
}

static Stack *stack_new_empty(stack_size_type size)
{
    Stack *s = (Stack *)malloc(sizeof(Stack));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    assert(s != NULL);
    s->base = (STACK_T *)calloc(size, sizeof(STACK_T));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    assert(s->base != NULL);
    s->top = s->base;
    s->size = size;
    return s;
}

static void stack_free(Stack **sp)
{
    assert(sp != NULL);
    assert(*sp != NULL);
    free((*sp)->base);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    free((*sp));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    *sp = NULL;
}

static void stack_clear(Stack *s)
{
    assert(s != NULL);
    memset(s->base, 0, s->size * sizeof(STACK_T));
    s->top = s->base;
}

static void stack_push(Stack *s, STACK_T elem, my_bool allow_auto_expan, cmp_ret_type (*cmp)(const STACK_T, const STACK_T))
{
    assert(s != NULL);
    if (cmp(elem, STACK_T_NULL) == 0)
    {
        return;
    }
    if (stack_is_full(s))
    {
        if (allow_auto_expan)
        {
            fprintf(stderr, "NOTE: stack is full, trying to reallocate.\n");
            stack_size_type used = s->top - s->base;
            s->base = (STACK_T *)realloc(s->base, (s->size << 1) * sizeof(STACK_T));
            //             STACK_T *temp = (STACK_T *)malloc((s->size << 1) * sizeof(STACK_T ));
            // #ifdef DEBUG_ALLOC_FREE_COUNT
            //             g_alloc_count++;
            // #endif
            //             if (temp == NULL)
            //             {
            //                 fprintf(stderr, "ERROR: stack overflow!\n");
            //                 return;
            //             }
            //             memcpy(temp, s->base, (s->top - s->base) * sizeof(STACK_T ));
            //             free(s->base);
            // #ifdef DEBUG_ALLOC_FREE_COUNT
            //             g_free_count++;
            // #endif
            //             s->base = temp;
            s->top = s->base + used;
            s->size <<= 1;
        }
        else
        {
            fprintf(stderr, "ERROR: stack overflow!\n");
            return;
        }
    }
    *(s->top) = elem;
    s->top++;
}

static STACK_T stack_pop(Stack *s)
{
    assert(s != NULL);
    if (!stack_is_empty(s))
    {

        STACK_T ret = *(s->top - 1);
        *(s->top - 1) = STACK_T_NULL;
        s->top--;
        return ret;
    }
    else
    {
        fprintf(stderr, "ERROR: empty stack!\n");
        return STACK_T_NULL;
    }
}

static STACK_T stack_get_top(const Stack *s)
{
    assert(s != NULL);
    if (!stack_is_empty(s))
    {
        return *(s->top - 1);
    }
    else
    {
        fprintf(stderr, "ERROR: empty stack!\n");
        return STACK_T_NULL;
    }
}

static void stack_print(
    const Stack *s,
    FILE *fp,
    const char *format,
    void (*elem_print)(FILE *, const STACK_T, cmp_ret_type (*)(const STACK_T, const STACK_T)),
    cmp_ret_type (*cmp)(const STACK_T, const STACK_T))
{
    assert(s != NULL);
    if (strcmp(format, "SINGLE_LINE") != 0 && strcmp(format, "MULTI_LINES") != 0)
    {
        fprintf(stderr, "ERROR: undefined format string: %s, expected SINGLE_LINE or MULTI_LINES.\n", format);
        return;
    }
    if (strcmp(format, "SINGLE_LINE") == 0)
    { /* may overflow */
        fprintf(fp, "Stack at: %p, size: %" PRId64 ", used: %" PRId64 ", content: ", s, s->size, stack_used_size(s));
        stack_size_type i;
        for (i = 0; s->base + i < s->top; ++i)
        {

            if (s->base + i == s->base)
            {
                elem_print(fp, *(s->base + i), cmp);
                fprintf(fp, "(base)");
            }
            else
            {
                fprintf(fp, " >> ");
                elem_print(fp, *(s->base + i), cmp);
            }
            if (s->base + i + 1 == s->top)
            {
                fprintf(fp, "(top)");
            }
        }

        for (; i < s->size; ++i)
        {
            if (i != 0)
                fprintf(fp, " >> STACK_T_NULL");
            else
                fprintf(fp, "STACK_T_NULL");
        }
        fprintf(fp, "\n");
    }
    if (strcmp(format, "MULTI_LINES") == 0)
    {
        fprintf(fp, "==================== Stack at %18p ====================\n", s);
        fprintf(fp, "================== used_size: %20" PRId64 " ==================\n", stack_used_size(s));
        fprintf(fp, "================= total_size: %20" PRId64 " ==================\n", s->size);
        stack_size_type i;
        for (i = s->size - 1; s->base + i >= s->top; i--)
        {
            fprintf(fp, "         STACK_T_NULL\n");
        }
        for (; s->base + i >= s->base; --i)
        {
            if (s->base + i + 1 == s->top)
            {
                fprintf(fp, "(top)  -> ");
            }
            if (s->base + i == s->base)
            {
                fprintf(fp, "(base) -> ");
            }
            if (s->base + i != s->top && s->base + i != s->base)
            {
                fprintf(fp, "        ");
            }
            elem_print(fp, *(s->base + i), cmp);
            fprintf(fp, "\n");
        }
        fprintf(fp, "========================= Stack Print: Over =========================\n");
    }
}

/******************************************************************************************/

static rbtree_size_type my_log_floor(const rbtree_size_type n)
{
    rbtree_size_type i = 0;
    rbtree_size_type ret = 1;
    while (ret < n)
    {
        ret *= 2;
        i++;
    }
    return i;
}
static void rbnode_set_value(RBNode *rbn_p, const T elem)
{
    if (rbn_p == NULL)
    {
        fprintf(stderr, "ERROR: cannot set data to a null RBNode!\n");
    }
#ifdef MY_TREE_MAP
    // rbn_p->data.key = elem.key;
    rbn_p->data.value = elem.value;
#endif
#ifdef MY_TREE_SET
    rbn_p->data = elem;
#endif
}

static color_type rbnode_get_color(const RBNode *rbn_p)
{
    if (rbn_p == NULL)
    {
        return BLACK;
    }
    return rbn_p->color;
}

static void rbnode_set_color(RBNode *rbn_p, color_type new_color)
{
    rbn_p->color = new_color;
}

static RBNode *rbnode_new(const T elem)
{
    RBNode *p = (RBNode *)calloc(1, sizeof(RBNode));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif

    assert(p != NULL);
    p->data = (T)(elem);
    return p;
}

static RBTree *rbtree_new_empty(void)
{
    RBTree *rbt_p = (RBTree *)calloc(1, sizeof(RBTree));
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_alloc_count++;
#endif
    assert(rbt_p != NULL);
    return rbt_p;
}

static RBTree *rbtree_new_from_array(const T *elem_arr, const rbtree_size_type num, cmp_ret_type (*elem_cmp)(const T, const T))
{
    RBTree *rbt_p = rbtree_new_empty();
    rbtree_size_type i;
    for (i = 0; i < num; ++i)
    {
        rbtree_insert_elem(rbt_p, elem_arr[i], elem_cmp);
    }
    return rbt_p;
}

static rbtree_size_type rbtree_elem_count_quick(const RBTree *rbt_p)
{
    assert(rbt_p != NULL);
    return rbt_p->node_count;
}

static RBNode *rbtree_search(const RBTree *rbt_p, const T elem, cmp_ret_type (*elem_cmp)(const T, const T))
{ //
    // 当你想用key查找时，给定的一个T，只需指定K即可，V随意
    assert(rbt_p != NULL);
    RBNode *temp = rbt_p->root; //从根节点开始
    while (temp != NULL && elem_cmp(elem, temp->data) != 0)
    {
        if (elem_cmp(elem, temp->data) < 0)
        {
            temp = temp->left;
        }
        else
        {
            temp = temp->right;
        }
    }
    return temp;
}

static void rbtree_left_rotate(RBTree *rbt_p, RBNode *x)
{
    /*对红黑树的节点(x)进行左旋转
    **      px                              px
    **      /                               /
    **     x                               y
    **    /  \      --(左旋)-->           / \           
    **   lx   y                          x  ry
    **      /  \                        / \
    **     ly   ry                     lx  ly
    */
    RBNode *y = x->right; //设置x的右孩子为y

    x->right = y->left; //将 “y的左孩子” 设为 “x的右孩子”
    if (y->left != NULL)
    { //如果y的左孩子非空，将 “x” 设为 “y的左孩子的父亲”
        y->left->parent = x;
    }

    y->parent = x->parent; //将 “x的父亲” 设为 “y的父亲”

    if (x->parent == NULL)
    { //如果 “x的父亲” 是空节点，则将y设为根节点
        rbt_p->root = y;
    }
    else
    {
        if (x->parent->left == x)
        { //如果 x是它父节点的左孩子，则将y设为“x的父节点的左孩子”
            x->parent->left = y;
        }
        else
        { //如果 x是它父节点的左孩子，则将y设为“x的父节点的左孩子”
            x->parent->right = y;
        }
    }

    y->left = x;   //将 “x” 设为 “y的左孩子”
    x->parent = y; //将 “x的父节点” 设为 “y”
}

static void rbtree_right_rotate(RBTree *rbt_p, RBNode *x)
{
    /* 对红黑树的节点(x)进行右旋
    **        px            px
    **        /             /
    **       x              y
    **      / \            / \
    **     y   rx        ly   x
    **    / \                / \
    **   ly  ry            ry   rx
    */
    RBNode *y = x->left; //设置x的左孩子为y

    x->left = y->right; //将 “y的右孩子” 设为 “x的左孩子”
    if (y->right != NULL)
    { //如果y的右孩子非空，将 “x” 设为 “y的右孩子的父亲”
        y->right->parent = x;
    }

    y->parent = x->parent; //将 “x的父亲” 设为 “y的父亲”
    if (x->parent == NULL)
    { //如果 “x的父亲” 是空节点，则将y设为根节点
        rbt_p->root = y;
    }
    else
    {
        if (x == x->parent->right)
        { //如果 x是它父节点的右孩子，则将y设为“x的父节点的右孩子”
            x->parent->right = y;
        }
        else
        { //如果 x是它父节点的左孩子，则将y设为“x的父节点的左孩子”
            x->parent->left = y;
        }
    }

    y->right = x;  //将 “x” 设为 “y的右孩子”
    x->parent = y; //将 “x的父节点” 设为 “y”
}

static void rbtree_insert_elem(RBTree *rbt_p, const T elem, cmp_ret_type (*elem_cmp)(const T, const T))
{
    RBNode *new_node = rbnode_new(elem);
    status s = rbtree_insert_node(rbt_p, new_node, elem_cmp);
    if (s == STATUS_NEED_FREE)
    {
        free(new_node);
#ifdef DEBUG_ALLOC_FREE_COUNT
        g_free_count++;
#endif
    }
}

static status rbtree_insert_node(RBTree *rbt_p, RBNode *rbn_p, cmp_ret_type (*elem_cmp)(const T, const T))
{
    assert(rbt_p != NULL);
    assert(rbn_p != NULL);

    // 先找到父亲节点
    RBNode *temp = rbt_p->root;
    RBNode *temp_parent = NULL; //当fast走到NULL，slow可以记录下
    my_bool last_dir_is_left = FALSE;
    while (temp != NULL)
    {
        temp_parent = temp;
        if (elem_cmp(rbn_p->data, temp->data) == 0)
        { //case 2: key已经存在了, 相当于更新value
            //此时要注意，如果这是从elem那边传过来的，可能需要free
            rbnode_set_value(temp, rbn_p->data);
            return STATUS_NEED_FREE;
        }
        else if ((elem_cmp(rbn_p->data, temp->data) < 0))
        {
            temp = temp->left;
            last_dir_is_left = TRUE;
        }
        else
        {
            temp = temp->right;
            last_dir_is_left = FALSE;
        }
    }
    rbnode_set_color(rbn_p, RED);
    if (temp_parent == NULL) //case 1: 根节点为NULL
    {                        //这里判断temp或者是temp_parent都无所谓
        rbt_p->root = rbn_p;
        rbnode_set_color(rbn_p, BLACK);
    }

    // 当根不是NULL，接下来无论如何，先插入再说，然后fixup调整就行了
    if (last_dir_is_left && temp_parent)
    {
        temp_parent->left = rbn_p;
    }
    if (!last_dir_is_left && temp_parent)
    {
        temp_parent->right = rbn_p;
    }
    rbn_p->parent = temp_parent;
    rbn_p->left = NULL;
    rbn_p->right = NULL;
    rbtree_insert_fixup(rbt_p, rbn_p);
    rbt_p->node_count++;
    return STATUS_OK;
}

static void rbtree_insert_fixup(RBTree *rbt_p, RBNode *rbn_p)
{
    assert(rbt_p != NULL);
    assert(rbn_p != NULL);
    if (rbnode_get_color(rbn_p->parent) == BLACK)
    { // 不管插入节点的parent是NULL，
        // 或者parent是黑色(case 3)
        // 或者插入的就是根
        //都在insert已经做过调整了
        return;
    }

    //case 4: parent是红
    RBNode *p, *g; //parent和grandparent
    while ((p = rbn_p->parent) && rbnode_get_color(rbn_p->parent) == RED)
    { //接下来必须先判定parent的左右才能拿到uncle，所以case 4.1写在了判定左右的里面，而且要写两次
        g = p->parent;
        if (p == g->left)
        { //parent在左，uncle在右
            register RBNode *u = g->right;
            if (rbnode_get_color(u) == RED)
            { //case 4.1: 当uncle是红色
                rbnode_set_color(g, RED);
                rbnode_set_color(p, BLACK);
                rbnode_set_color(u, BLACK);
                rbn_p = g;
                continue;
            }
            else
            { //case 4.2: 当uncle是黑色
                if (rbn_p == p->right)
                { //case 4.2.2: 插入节点是其parent的右，即uncle黑左右
                    register RBNode *temp;
                    rbtree_left_rotate(rbt_p, p); //左旋
                    temp = p;
                    p = rbn_p;
                    rbn_p = temp;
                    //TODO 是交换还是直接改变, 源码是交换; 先赋值再旋转还是先旋转再赋， 源码是先旋转？
                }

                //case 4.2.1: 插入节点是其parent的左，即uncle黑左左
                rbnode_set_color(p, BLACK);
                rbnode_set_color(g, RED);
                rbtree_right_rotate(rbt_p, g); //右旋
            }
        }
        else
        { //parent在右，uncle在左
            register RBNode *u = g->left;
            if (rbnode_get_color(u) == RED)
            { //还是case 4.1: 当uncle是红色
                rbnode_set_color(g, RED);
                rbnode_set_color(p, BLACK);
                rbnode_set_color(u, BLACK);
                rbn_p = g;
                continue;
            }
            else
            { //还是case 4.2: 当uncle是黑色
                if (rbn_p == p->left)
                { //case 4.2.3: 插入节点是其parent的左，即uncle黑右左
                    register RBNode *temp;
                    rbtree_right_rotate(rbt_p, p); //右旋
                    temp = p;
                    p = rbn_p;
                    rbn_p = temp;
                }

                //case 4.2.4: 插入节点是其parent的右，即uncle黑右右
                rbnode_set_color(p, BLACK);
                rbnode_set_color(g, RED);
                rbtree_left_rotate(rbt_p, g); //左旋
            }
        }
    }
    rbnode_set_color(rbt_p->root, BLACK);
    return;
}

static void rbtree_delete_elem(RBTree *rbt_p, T elem, cmp_ret_type (*elem_cmp)(const T, const T))
{ //map模式下，如果想删除某个KV，所给的T中只需K正确即可，V随意不起作用
    rbtree_delete_node(rbt_p, rbtree_search(rbt_p, elem, elem_cmp));
}

static void rbtree_delete_node(RBTree *rbt_p, RBNode *node)
{ //TODO和源码不一样
    if (node == NULL)
    {
        fprintf(stderr, "NOTE: elem not found or node is null.\n");
        return;
    }
    RBNode *child;
    RBNode *p;
    color_type temp_color;
    if (!node->left)
    {
        child = node->right;
    }
    else if (!node->right)
    {
        child = node->left;
    }
    else
    {
        RBNode *old = node;
        RBNode *left;

        node = node->right;
        while ((left = node->left) != NULL)
            node = left;
        child = node->right;
        p = node->parent;
        temp_color = rbnode_get_color(node);

        if (child)
            child->parent = p;
        if (p == old)
        {
            p->right = child;
            p = node;
        }
        else
            p->left = child;

        node->color = old->color;
        node->right = old->right;
        node->left = old->left;

        if (old->parent)
        {
            if (old->parent->left == old)
                old->parent->left = node;
            else
                old->parent->right = node;
        }
        else
            rbt_p->root = node;

        old->left->parent = node;
        if (old->right)
            old->right->parent = node;
        if (temp_color == BLACK)
            rbtree_delete_fixup(rbt_p, child, p);
        return;
    }
    p = node->parent;
    temp_color = rbnode_get_color(node);

    if (child)
        child->parent = p;
    if (p)
    {
        if (p->left == node)
            p->left = child;
        else
            p->right = child;
    }
    else
        rbt_p->root = child;

    if (temp_color == BLACK)
        rbtree_delete_fixup(rbt_p, child, p);
    /**************************/
    /* RBNode *child = NULL;
    color_type temp_color;

    if ((node->left != NULL) && (node->right != NULL))
    {                           //被删除节点的左右孩子都非空
        RBNode *replace = node; //被删除节点的后继节点，其实也是被删节点的右子树的最左最下端节点
        //用它取代被删节点，然后free被删节点即可

        replace = replace->right;
        while (replace->left != NULL)
        { //一路向左
            replace = replace->left;
        }

        if (node->parent != NULL)
        { //node不是根

            if (node->parent->left == node)
            {
                node->parent->left = replace;
            }
            else
            {
                node->parent->left = replace;
            }
        }
        else
        { //直接更新根
            rbt_p->root = replace;
        }

        child = replace->right; //replace是最左，可能有右孩子，但是不会有左孩子
        temp_color = rbnode_get_color(replace);

        if (replace->parent == node)
        { //被删节点右孩子直接是后继节点，也就是被删节点直接是后继节点的父亲
            // replace->parent = replace;
            //TODO
        }
        else
        {
            if (child != NULL)
            {
                child->parent = replace->parent;
            }
            replace->parent->left = child; //这里一定是左，因为replace->parent > child > replace

            //把node替换成replace
            replace->right = node->right;
            node->right->parent = replace;
        }

        replace->parent = node->parent;
        replace->color = node->color;
        replace->left = node->left;
        node->left->parent = replace;

        if (temp_color == BLACK)
        {
            rbtree_delete_fixup(rbt_p, child, replace->parent);
            free(node);
#ifdef DEBUG_ALLOC_FREE_COUNT
            g_free_count++;
#endif
            return;
        }
    }

    if (node->left != NULL)
    { //左非空、右空
        child = node->left;
    }
    else
    { //左空、右非空 或者 两个空
        child = node->right;
    }

    temp_color = node->color;

    if (child != NULL)
    {
        child->parent = node->parent;
    }

    if (node->parent != NULL)
    { //node不是根
        if (node->parent->left == node)
        {
            node->parent->left = child;
        }
        else
        {
            node->parent->right = child;
        }
    }
    else
    {
        rbt_p->root = child;
    }

    if (temp_color == BLACK)
    {
        rbtree_delete_fixup(rbt_p, child, node->parent);
        free(node);
        rbt_p->node_count--;
#ifdef DEBUG_ALLOC_FREE_COUNT
        g_free_count++;
#endif
    } */
}

static void rbtree_delete_fixup(RBTree *rbt_p, RBNode *node, RBNode *p)
{
    RBNode *other;
    while (rbnode_get_color(node) == BLACK && node != rbt_p->root)
    { //case 2: 替换节点是黑色
        if (p->left == node)
        { //case 2.1: 替换节点是其父亲的左孩子

            other = p->right;
            if (rbnode_get_color(other) == RED)
            { //case 2.1.1: 替换节点的兄弟节点是红色
                rbnode_set_color(other, BLACK);
                rbnode_set_color(p, RED);
                rbtree_left_rotate(rbt_p, p);

                //接下来按照2.1.2.3处理
                other = p->right; //TODO
            }

            //case 2.1.2: 替换节点的兄弟节点是黑色

            if (rbnode_get_color(other->left) == BLACK && rbnode_get_color(other->right) == BLACK)
            { //case 2.1.2.3: 替换节点的兄弟节点的左右子节点都是黑色
                rbnode_set_color(other, RED);

                node = p;
                p = node->parent; //这两行是将p作为新的替换节点
            }
            else
            {
                if (rbnode_get_color(other->right) == BLACK)
                { //case 2.1.2.2: 替换节点的兄弟节点的孩子左红右黑
                    rbnode_set_color(other, RED);
                    if (other->left != NULL)
                        rbnode_set_color(other->left, BLACK);
                    rbtree_right_rotate(rbt_p, other);

                    //接下来按照2.1.2.1处理
                    other = p->right; //TODO
                }

                //case 2.1.2.1: 替换节点的兄弟节点的孩子：左任意、右红
                rbnode_set_color(other, rbnode_get_color(p));
                rbnode_set_color(p, BLACK);
                if (other->right != NULL)
                    rbnode_set_color(other->right, BLACK);
                rbtree_left_rotate(rbt_p, p);

                node = rbt_p->root; //TODO
                break;
            }
        }
        else
        { //case 2.2: 替换节点是其父亲的右孩子

            other = p->left;
            if (rbnode_get_color(other) == RED)
            { // Case 2.2.1: 替换节点的兄弟节点是红色
                rbnode_set_color(other, BLACK);
                rbnode_set_color(p, RED);
                rbtree_right_rotate(rbt_p, p);

                //接下来按照2.2.2.3处理
                other = p->left; //TODO
            }

            //case 2.2.2: 替换节点的兄弟节点是黑色

            if (rbnode_get_color(other->left) == BLACK && rbnode_get_color(other->right) == BLACK)
            { //case 2.2.2.3 替换节点的兄弟节点的孩子都是黑色
                rbnode_set_color(other, RED);

                node = p;
                p = node->parent; //这两行是将p作为新的替换节点
            }
            else
            {
                if (rbnode_get_color(other->left) == BLACK)
                { //case 2.2.2.2: 替换节点的兄弟节点的孩子左黑右红
                    rbnode_set_color(other, RED);
                    if (other->right != NULL)
                        rbnode_set_color(other->right, BLACK);
                    rbtree_left_rotate(rbt_p, other);

                    //接下来按照2.2.2.1处理
                    other = p->left; //TODO
                }

                //case 2.2.2.1: 替换节点的兄弟节点的孩子：左红、右任意
                rbnode_set_color(other, rbnode_get_color(p));
                rbnode_set_color(p, BLACK);
                if (other->left != NULL)
                    rbnode_set_color(other->left, BLACK);
                rbtree_right_rotate(rbt_p, p);

                node = rbt_p->root; //TODO
                break;
            }
        }
    }
    if (node)
    {
        rbnode_set_color(node, BLACK);
    }
}

static RBNode *rbtree_first(RBTree *rbt_p)
{
    assert(rbt_p != NULL);
    RBNode *ret;
    ret = rbt_p->root;
    if (ret == NULL)
    {
        return NULL;
    }
    while (ret->left != NULL)
    {
        ret = ret->left;
    }
    return ret;
}

static RBNode *rbtree_last(RBTree *rbt_p)
{
    assert(rbt_p != NULL);
    RBNode *ret;
    ret = rbt_p->root;
    if (ret == NULL)
    {
        return NULL;
    }
    while (ret->right != NULL)
    {
        ret = ret->right;
    }
    return ret;
}

static RBNode *rbtree_next(RBNode *node)
{
    assert(node != NULL);
    if (node->parent == NULL && node->left == NULL && node->right == NULL)
    { //只有一个节点
        return NULL;
    }

    if (node->right != NULL)
    { //右非空，则右子树的最左端就是下一个
        node = node->right;
        while (node->left != NULL)
        {
            node = node->left;
        }
        return node;
    }

    //右空，下一个只能从parent找
    RBNode *p = node->parent;
    while (p != NULL && node == p->right)
    { //node是其parent的右孩子，意味着node>parent，就得一直找
        node = p;
        p = node->parent;
    }
    return p;
}

static RBNode *rbtree_prev(RBNode *node)
{
    assert(node != NULL);
    if (node->parent == NULL && node->left == NULL && node->right == NULL)
    { //只有一个节点
        return NULL;
    }

    if (node->left != NULL)
    { //左非空，则左子树的最右端就是上一个
        node = node->left;
        while (node->right != NULL)
        {
            node = node->right;
        }
        return node;
    }

    //左空，上一个只能从parent找
    RBNode *p = node->parent;
    while (p != NULL && node == p->left)
    { //node是其parent的左孩子，意味着node<parent，就得一直找
        node = p;
        p = node->parent;
    }
    return p;
}

static void rbtree_destroy(RBTree **rbt_pp)
{
    assert(rbt_pp != NULL);
    assert(*rbt_pp != NULL);
    RBNode *top = (*rbt_pp)->root;
    RBNode *pop;
    Stack *s = stack_new_empty((stack_size_type)(my_log_floor((*rbt_pp)->node_count) + 1));
    stack_push(s, top, TRUE, stack_cmp);
    while (!stack_is_empty(s))
    {
        top = stack_get_top(s);
        while (top) //while or if
        {
            top = top->left;
            stack_push(s, top, TRUE, stack_cmp);
            //本文中，空元素入栈无效
        }
        top = stack_get_top(s);
        if (top->right == NULL)
        {
            pop = stack_pop(s);
            if (stack_used_size(s) == 0)
            {
                free(pop);
#ifdef DEBUG_ALLOC_FREE_COUNT
                g_free_count++;
#endif
                break;
            }
            if (top == top->parent->left)
                top->parent->left = NULL;
            else if (top == top->parent->right)
                top->parent->right = NULL;
            free(pop);
#ifdef DEBUG_ALLOC_FREE_COUNT
            g_free_count++;
#endif
        }
        else
        {
            stack_push(s, top->right, TRUE, stack_cmp);
        }
    }
    stack_free(&s);
    free(*rbt_pp);
#ifdef DEBUG_ALLOC_FREE_COUNT
    g_free_count++;
#endif
    *rbt_pp = NULL;
}

static void rbtree_preorder_traverse(const RBTree *rbt_p, status (*visit)(RBNode *))
{
    assert(rbt_p != NULL);
    RBNode *temp = rbt_p->root;
    Stack *s = stack_new_empty((stack_size_type)(my_log_floor(rbt_p->node_count)) + 1);
    while (temp || !stack_is_empty(s))
    {
        if (temp)
        {
            if (!visit(temp))
            {
                return;
            }
            stack_push(s, temp, TRUE, stack_cmp);
            temp = temp->left;
        }
        else
        {
            temp = stack_pop(s);
            temp = temp->right;
        }
    }
    stack_free(&s);
}

static void rbtree_inorder_traverse(const RBTree *rbt_p, status (*visit)(RBNode *))
{
    assert(rbt_p != NULL);
    Stack *s = stack_new_empty((stack_size_type)(my_log_floor(rbt_p->node_count)) + 1);
    // RBNode *temp;
    // stack_push(s, rbt_p->root, FALSE, stack_cmp);
    // while (!stack_is_empty(s))
    // {
    //     while (temp = stack_get_top(s))
    //     {
    //         stack_push(s, temp->left, TRUE, stack_cmp);
    //     }
    //     stack_pop(s); //最左节点的左孩子是空，需要退出
    //但是本文件中stack中push一个null跟没push一样的，所以这个思路不好
    //     if (!stack_is_empty(s))
    //     {
    //         temp = stack_pop(s);
    //         if (!visit(temp))
    //         {
    //             return;
    //         }
    //         stack_push(s, temp->right, TRUE, stack_cmp);
    //     }
    // }
    RBNode *temp = rbt_p->root;
    while (temp || !stack_is_empty(s))
    {
        if (temp) //while or if
        {
            stack_push(s, temp, TRUE, stack_cmp);
            temp = temp->left;
        }
        else
        // while (!stack_is_empty(s))
        {
            temp = stack_pop(s);
            if (!visit(temp))
            {
                return;
            }
            temp = temp->right;
        }
    }
    stack_free(&s);
}

static void rbtree_postorder_traverse(const RBTree *rbt_p, status (*visit)(RBNode *))
{
    assert(rbt_p != NULL);
    RBNode *cur = rbt_p->root;
    RBNode *prev = NULL;
    Stack *s = stack_new_empty((stack_size_type)(my_log_floor(rbt_p->node_count)) + 1);
    stack_push(s, rbt_p->root, TRUE, stack_cmp); //因此对于任一结点P，先将其入栈
    while (!stack_is_empty(s))
    {
        cur = stack_get_top(s);
        if ((cur->left == NULL && cur->right == NULL) || (prev != NULL && (prev == cur->left || prev == cur->right)))
        { //P不存在左孩子和右孩子，则可以直接访问它
            //或者P存 在左孩子或者右孩子，但是其左孩子和右孩子都已被访问过了，则同样可以直接访问该结点
            if (!visit(cur))
                return;
            stack_pop(s);
            prev = cur;
        }
        else
        { //否则左右都入栈
            if (cur->right)
            {
                stack_push(s, cur->right, TRUE, stack_cmp);
            }
            if (cur->left)
            {
                stack_push(s, cur->left, TRUE, stack_cmp);
            }
        }
    }
    stack_free(&s);
}

static void rbtree_visual_print(FILE *fp, const RBTree *rbt_p, RBNode *node, rbtree_size_type high, rbtree_size_type n, void (*elem_print)(FILE *, const T))
{
    if (rbt_p != NULL)
    {
        rbtree_visual_print(fp, rbt_p, node->right, high + 7, n + 1, elem_print);
        rbtree_size_type i = 0;
        while (i != high)
        {
            if (i % 7 == 0)
            {
                if (i >= 7 * n)
                    printf("|");
                else
                    printf(" ");
            }
            else
                printf(" ");
            i++;
        }
        if (rbnode_get_color(node) == RED)
            fprintf(fp, "\033[1m\033[47;31|-<R>");
        else
            fprintf(fp, "\033[1m\033[47;30|-<B>");
        // printf("%2d-\n", node->key);
        elem_print(fp, node->data);
        fprintf(fp, "\033[0m\n");

        rbtree_visual_print(fp, rbt_p, node->left, high + 7, n + 1, elem_print);
    }
    else
    {
        rbtree_size_type i = 0;
        while (i != high)
        {
            if (i % 7 == 0)
            {
                if (i >= 7 * n)
                    printf("|");
                else
                    printf(" ");
            }
            else
                printf(" ");
            i++;
        }
        printf("\033[1m\033[47;30|-NIL\033[0m\n");
    }
}
/**************************************/
status visit(RBNode *p)
{
    elem_print_imp(stdout, p->data);
    fprintf(stdout, ", ");
    return 1;
}
int main(int argc, char const *argv[])
{
    fprintf(stdout, "argc=%d\n", argc);
    fprintf(stdout, "argv[0]=%s\n", argv[0]);
    KV e0 = {(K)23, "A"};
    KV e1 = {(K)11, "B"};
    KV e2 = {(K)29, "C"};
    KV e3 = {(K)36, "D"};
    KV e4 = {(K)74, "E"};
    KV e5 = {(K)52, "F"};
    KV e6 = {(K)66, "G"};
    KV e7 = {(K)79, "H"};
    KV e8 = {(K)81, "I"};
    KV e9 = {(K)11, "J"};

    KV e_array[] = {e0, e1, e2, e3, e4, e5, e6, e7, e8, e9};

    RBTree *rbt_p = rbtree_new_empty();
    rbtree_size_type i;
    for (i = 0; i < 10; ++i)
    {
        rbtree_insert_elem(rbt_p, e_array[i], elem_compare);
        fprintf(stdout, "rbtree elem count: %" PRId64 "\n", rbtree_elem_count_quick(rbt_p));
    }

    RBNode *cur;
    for (cur = rbtree_first(rbt_p);; cur = rbtree_next(cur))
    {

        elem_print_imp(stdout, cur->data);
        fprintf(stdout, ", ");
        if (cur == rbtree_last(rbt_p))
        {
            break;
        }
    }
    fprintf(stdout, "\n");

    rbtree_preorder_traverse(rbt_p, visit);
    fprintf(stdout, "\n");
    rbtree_inorder_traverse(rbt_p, visit);
    fprintf(stdout, "\n");
    rbtree_postorder_traverse(rbt_p, visit);
    fprintf(stdout, "\n");

    // rbtree_visual_print(stdout, rbt_p, rbt_p->root, 0, 0, elem_print_imp);

    fprintf(stdout, "\ntest search: \n");
    elem_print_imp(stdout, rbtree_search(rbt_p, (T){(K)11, NULL}, elem_compare)->data);
    fprintf(stdout, "\n");
    elem_print_imp(stdout, rbtree_search(rbt_p, (T){(K)79, NULL}, elem_compare)->data);
    fprintf(stdout, "\ntest search: OVER\n");

    fprintf(stdout, "\ntest delete: \n");
    rbtree_delete_elem(rbt_p, (T){(K)11, NULL}, elem_compare);
    fprintf(stdout, "rbtree elem count: %" PRId64 "\n", rbtree_elem_count_quick(rbt_p));
    fprintf(stdout, "after delete, tree is: (inorder)\n");
    rbtree_inorder_traverse(rbt_p, visit);
    fprintf(stdout, "\ntest delete: OVER \n");

    fprintf(stdout, "\ntest delete null elem: \n");
    rbtree_delete_elem(rbt_p, T_NULL, elem_compare);
    fprintf(stdout, "rbtree elem count: %" PRId64 "\n", rbtree_elem_count_quick(rbt_p));
    fprintf(stdout, "after delete, tree is: (inorder)\n");
    rbtree_inorder_traverse(rbt_p, visit);
    fprintf(stdout, "\ntest delete null elem: OVER \n");

    fprintf(stdout, "\ntest insert null elem: \n");
    rbtree_insert_elem(rbt_p, T_NULL, elem_compare);
    fprintf(stdout, "rbtree elem count: %" PRId64 "\n", rbtree_elem_count_quick(rbt_p));
    fprintf(stdout, "after insert, tree is: (inorder)\n");
    rbtree_inorder_traverse(rbt_p, visit);
    fprintf(stdout, "\ntest insert null elem: OVER \n\n");

    rbtree_delete_elem(rbt_p, e2, elem_compare);
    rbtree_inorder_traverse(rbt_p, visit);
    fprintf(stdout, "\n");

    rbtree_delete_elem(rbt_p, e9, elem_compare);
    rbtree_inorder_traverse(rbt_p, visit);
    fprintf(stdout, "\n");

    rbtree_delete_elem(rbt_p, e3, elem_compare);
    rbtree_inorder_traverse(rbt_p, visit);
    fprintf(stdout, "\n");

    rbtree_delete_elem(rbt_p, e6, elem_compare);
    rbtree_inorder_traverse(rbt_p, visit);
    fprintf(stdout, "\n");

    rbtree_delete_elem(rbt_p, e8, elem_compare);
    rbtree_inorder_traverse(rbt_p, visit);
    fprintf(stdout, "\n");

    rbtree_delete_elem(rbt_p, e4, elem_compare);
    rbtree_inorder_traverse(rbt_p, visit);
    fprintf(stdout, "\n");

    rbtree_delete_elem(rbt_p, e5, elem_compare);
    rbtree_inorder_traverse(rbt_p, visit);
    fprintf(stdout, "\n");

    rbtree_delete_elem(rbt_p, e1, elem_compare);
    rbtree_inorder_traverse(rbt_p, visit);
    fprintf(stdout, "\n");

    fprintf(stdout, "*************\n");
    rbtree_preorder_traverse(rbt_p, visit);
    fprintf(stdout, "\n");
    rbtree_inorder_traverse(rbt_p, visit);
    fprintf(stdout, "\n");
    rbtree_postorder_traverse(rbt_p, visit);
    fprintf(stdout, "\n");
    rbtree_delete_elem(rbt_p, e0, elem_compare);
    rbtree_inorder_traverse(rbt_p, visit);
    fprintf(stdout, "\n");

    rbtree_delete_elem(rbt_p, e7, elem_compare);
    rbtree_inorder_traverse(rbt_p, visit);
    fprintf(stdout, "\n");

    rbtree_destroy(&rbt_p);
    if (g_alloc_count == g_free_count)
    {
        fprintf(stderr, "\033[1m\033[;32malloc_count: %" PRIu64 ", free_count: %" PRIu64 "\033[0m\n", g_alloc_count, g_free_count);
    }
    else
    {
        fprintf(stderr, "\033[1m\033[;31malloc_count: %" PRIu64 ", free_count: %" PRIu64 "\033[0m\n", g_alloc_count, g_free_count);
    }
    return 0;
}

#endif
#undef T
#undef T_NULL