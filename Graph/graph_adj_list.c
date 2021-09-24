#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <malloc.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stddef.h>

typedef void *data_t;
typedef int weight_t;
typedef int index_t; //顶点的索引或数量类型
typedef int status;

#ifndef USE_IN_DEGREE
#define USE_IN_DEGREE
#endif
//
// #undef USE_IN_DEGREE //注释这一行来 决定 是否在每个顶点处记录入度
//

typedef struct edge
{
    index_t dest_vertex;
    weight_t weight;
    struct edge *next;
} Edge;

typedef struct vertex
{
#ifdef USE_IN_DEGREE
    index_t in_degree;
#endif
    data_t data;
    Edge *link;
} Vertex;

#define MAX_VERTEX_NUM 1024

#ifndef DEBUG_DYNAMIC_MEM_CHECK
#define DEBUG_DYNAMIC_MEM_CHECK
#endif

#ifdef DEBUG_DYNAMIC_MEM_CHECK
int g_alloc_count = 0;
int g_free_count = 0;
#endif

#define new_edge                  \
    (Edge *)malloc(sizeof(Edge)); \
    {                             \
        g_alloc_count++;          \
    }                             \
    while (0)

#define new_vertex_array(n)                 \
    (Vertex *)malloc((n) * sizeof(Vertex)); \
    {                                       \
        g_alloc_count++;                    \
    }                                       \
    while (0)

#ifndef MY_DATA_TYPE
#define MY_DATA_TYPE
#endif
#ifdef MY_DATA_TYPE
#define MAX_STR_LEN 128

void fill_data(Vertex *vp, FILE *fp)
{ //Overwrite
    vp->data = (char *)malloc(MAX_STR_LEN * sizeof(char));
#ifdef DEBUG_DYNAMIC_MEM_CHECK
    g_alloc_count++;
#endif
    fscanf(fp, "%s\n", (char *)(vp->data)); // 这个\n加不加都行
    // fgets((char *)(vp->data), MAX_STR_LEN, fp);
}

void visit_data(Vertex *vp)
{
    fprintf(stderr, "%s ", (char *)(vp->data)); //必须强制转换，因为用void*泛型时，这里不知道是什么指针，无法解引用
}
#endif

Vertex *create_graph(index_t vertex_num, index_t edge_num, FILE *fp)
{
    index_t i, v_start, v_end;
    weight_t weight;
    Edge *p, *q;
    Vertex *ret = new_vertex_array(vertex_num);
    for (i = 0; i < vertex_num; ++i)
    {
#ifdef USE_IN_DEGREE
        ret[i].in_degree = 0; //默认为0
#endif
        fill_data(ret + i, fp);
        ret[i].link = NULL;
    }

    for (i = 0; i < edge_num; ++i)
    {
        fscanf(fp, "%d %d %d ", &v_start, &v_end, &weight);
        p = new_edge;
        p->dest_vertex = v_end;
        p->weight = weight;
        p->next = NULL;
        if (ret[v_start].link == NULL)
            ret[v_start].link = p; //第v_start个链表只有头节点
        else
        {
            q = ret[v_start].link;
            while (q->next) //找到末尾
                q = q->next;
            q->next = p;
        }
#ifdef USE_IN_DEGREE
        ret[v_end].in_degree++;
#endif
    }
    return ret;
}

void my_free(void *p)
{
    free(p);
#ifdef DEBUG_DYNAMIC_MEM_CHECK
    g_free_count++;
#endif
}

void destroy_graph(Vertex *vp, index_t vertex_num)
{
    index_t i;
    Edge *p, *q;
    for (i = 0; i < vertex_num; ++i)
    {
        p = vp[i].link;
        if (p != NULL)
        {
            do
            {
                q = p;
                p = p->next;
                my_free(q);
            } while (p != NULL);
        }
        my_free(vp[i].data);
    }
    my_free(vp);
}

void travel_depth_first(Vertex *vp, index_t vertex_num, index_t edge_num, void (*visit)(Vertex *))
{
    index_t i, top = -1;
    Edge *stack_edge_p[edge_num];
    int stack_pop_flag = 0;
    int visited[vertex_num]; //用变量定义长度， 不可同时赋初值
    memset(visited, 0, sizeof(visited));

    for (i = 0; i < vertex_num; ++i)
    {
        if (visited[i] == 0)
        {
            visit(vp + i);
            visited[i] = 1;
            Edge *adj_edge_p = vp[i].link;
            index_t cur_v_index = i;
            do
            {
                while (adj_edge_p != NULL)
                {
                    if (!stack_pop_flag) //退栈得到的边,不需要重定位到顶点的第一附属边,
                        adj_edge_p = vp[cur_v_index].link;
                    while (adj_edge_p != NULL && visited[adj_edge_p->dest_vertex]) //如果已经访问过了, 就跳过
                        adj_edge_p = adj_edge_p->next;
                    if (adj_edge_p != NULL)
                    {
                        visit(vp + (adj_edge_p->dest_vertex)); //碰见一个边(它指向的顶点没访问过)就访问
                        visited[adj_edge_p->dest_vertex] = 1;  //标记
                        stack_edge_p[++top] = adj_edge_p;      //入栈
                        stack_pop_flag = 0;                    //入栈标志
                        cur_v_index = adj_edge_p->dest_vertex;
                    }
                };
                if (top >= 0)
                {                                           //防止出现最后一次退栈访问-1
                    adj_edge_p = stack_edge_p[top--]->next; //出栈
                    stack_pop_flag = 1;                     //出栈标志
                }
            } while (!(top < 0 && adj_edge_p == NULL));
        }
    }
}

void travel_breadth_first(Vertex *vp, index_t vertex_num, index_t edge_num, void (*visit)(Vertex *))
{
    index_t i, front = -1, rear = -1;
    int visited[edge_num];
    Edge *cur_edge_p, *queue_edge_p[edge_num];
    memset(visited, 0, sizeof(visited));

    for (i = 0; i < vertex_num; ++i)
    {
        if (visited[i] == 0)
        {
            visit(vp + i);
            visited[i] = 1;
            queue_edge_p[++rear] = vp[i].link;
            while (rear > front)
            {
                cur_edge_p = queue_edge_p[++front];
                while (cur_edge_p)
                {
                    if (visited[cur_edge_p->dest_vertex] == 0)
                    {
                        visit(vp + cur_edge_p->dest_vertex);
                        visited[cur_edge_p->dest_vertex] = 1;
                        queue_edge_p[++rear] = vp[cur_edge_p->dest_vertex].link; //当前被访问顶点的紧接第一边入队
                    }
                    cur_edge_p = cur_edge_p->next;
                }
            }
        }
    }
}

#ifdef USE_IN_DEGREE
status topological_sort(Vertex *vp, index_t vertex_num, Vertex (**ret)[vertex_num])
//Activity On Vertex Network
{ //返回0说明有回路,不成立. 非0说明成功
    //巧妙之处在于，原地借助了入度已经变成了0的空间，作为栈
    Edge *p;
    index_t i, j, k, top = -1;
    for (i = 0; i < vertex_num; i++)
    {
        if (vp[i].in_degree == 0)
        {
            vp[i].in_degree = top;
            top = i;
        }
    }

    for (i = 0; i < vertex_num; ++i)
    {
        if (top == -1)
            return 0;
        else
        {
            j = top;
            top = vp[top].in_degree;
            ret[i] = vp + j; //输出个这个节点的地址

            p = vp[j].link;
            while (p != NULL)
            {
                k = p->dest_vertex;
                if (--(vp[k].in_degree) == 0) //以这个边为终点的边的入度要减少
                {                             //新的入度为0的顶点要入栈
                    vp[k].in_degree = top;
                    top = k;
                }
                p = p->next;
            }
        }
    }
    return 1;
}
#endif

#ifdef USE_IN_DEGREE
status critical_path(Vertex *vp, index_t vertex_num, index_t edge_num, Vertex (*(*ret))[vertex_num])
//Activity On Edge Network，顶点代表事件，边代表活动
{
    memset(ret, 0, vertex_num * sizeof(Vertex *)); //要不要清空返回区? 清空吧

    weight_t ee[vertex_num]; //事件发生最早时间，只有进入v的活动都结束了，v代表的事件才能发生，因此这个表示的是到v的最长路径和
    weight_t le[vertex_num]; //事件发生最晚时间，不推迟从v出发的活动的时间，因此这个表示的是：后面节点的这个值减去路径，使得前面节点的这个值尽可能小
    weight_t e[edge_num];    //活动最早开始时间，即顶点最早开始了，顶点引出的边才能开始，因此：某顶点在ee中的值，即从这点出发的每一条边在e中的值
    weight_t l[edge_num];    //活动最晚开始时间，要保证以某边为结尾的顶点v（事件）的最晚发生时间不被拖后。因此le中某顶点的值，减去以该顶点为终点的边的权值，即这些边在l的值

    Edge *p;
    index_t i, j, k;

    for (i = 0; i < vertex_num; ++i)
        ee[i] = 0;
    for (i = 0; i < vertex_num - 1; ++i)
    { //计算ee[]
        p = vp[i].link;
        while (p != NULL)
        {
            j = p->dest_vertex;            //这句话就显示出，p是i到j的一条边
            if (ee[j] < ee[i] + p->weight) //由i更新出来的累计路径和更长
                ee[j] = ee[i] + p->weight;
            p = p->next;
        } //体会逐渐累加的感觉
    }
    for (i = 0; i < vertex_num; ++i)
        le[i] = ee[vertex_num - 1];
    for (i = vertex_num - 2; i >= 0; --i)
    {
        p = vp[i].link;
        while (p != NULL)
        {
            j = p->dest_vertex;
            if (le[j] - p->weight < le[i]) //使j前面的i的le尽可能小
                le[i] = le[j] - p->weight;
            p = p->next;
        }
    }

    k = 0; //边的编号是从第一个顶点第一个边开始，先把第一个顶点的边算完，再算下一个顶点的
    index_t ret_pos = 0;
    for (i = 0; i < vertex_num - 1; ++i)
    {
        p = vp[i].link;
        while (p != NULL)
        {
            e[k] = ee[i]; //顶点i引出的边
            j = p->dest_vertex;
            l[k] = le[j] - p->weight;
            if (l[k] == e[k])            //关键活动
                ret[ret_pos++] = vp + i; //输出关键路径上的顶点指针
            p = p->next;
            k++;
        }
    }
    p = ((Vertex *)(ret[--ret_pos]))->link;
    while (p != NULL)
    {
        if (p->dest_vertex == vertex_num - 1)
        {
            ret[++ret_pos] = vp + vertex_num - 1;
            return 1;
        }
        p = p->next;
    }
    return 0;
}
#endif

void print_naive(Vertex *vp, index_t vertex_num, void (*visit)(Vertex *));
int main(int argc, char const *argv[])
{
    freopen("in.txt", "r", stdin);

    printf("**************************\n"); //test 1
    Vertex *g1 = create_graph(9, 22, stdin);

    print_naive(g1, 9, visit_data);

    travel_depth_first(g1, 9, 22, visit_data);
    printf("\n");

    travel_breadth_first(g1, 9, 22, visit_data);
    printf("\n");

    destroy_graph(g1, 9);

    printf("**************************\n"); //test 2

    Vertex *g2 = create_graph(6, 9, stdin);

    print_naive(g2, 6, visit_data);

    travel_depth_first(g2, 6, 9, visit_data);
    printf("\n");

    travel_breadth_first(g2, 6, 9, visit_data);
    printf("\n");

    destroy_graph(g2, 6);

    printf("**************************\n"); //test 3

    Vertex *g3 = create_graph(7, 8, stdin);

    print_naive(g3, 7, visit_data);

#ifdef USE_IN_DEGREE
    Vertex *ret[7];
    index_t i;
    if (topological_sort(g3, 7, ret))
        for (i = 0; i < 7; ++i)
            visit_data(ret[i]);
    printf("\n");
#endif

    destroy_graph(g3, 7);

    printf("**************************\n"); //test 4
    Vertex *g4 = create_graph(7, 10, stdin);

    print_naive(g4, 7, visit_data);

#ifdef USE_IN_DEGREE
    Vertex *ret2[7];
    if (critical_path(g4, 7, 10, ret2))
        for (i = 0; ret2[i] != NULL; ++i)
        {
            visit_data(ret2[i]);
        }
    printf("\n");
#endif

    destroy_graph(g4, 7);

#ifdef DEBUG_DYNAMIC_MEM_CHECK
    fprintf(stderr, "g_alloc_count: %d, g_free_count: %d.\n", g_alloc_count, g_free_count);
#endif
    return 0;
}

void print_naive(Vertex *vp, index_t vertex_num, void (*visit)(Vertex *))
{ //朴素打印
    index_t i;
    Edge *p;
    for (i = 0; i < vertex_num; ++i)
    {
#ifdef USE_IN_DEGREE
        fprintf(stderr, "(in_deg: %d) ", vp[i].in_degree);
#endif
        visit(vp + i);
        p = vp[i].link;
        while (p != NULL)
        {
            fprintf(stderr, "-> (v: %d| w: %d)", p->dest_vertex, p->weight);
            p = p->next;
        }
        fprintf(stderr, "\n");
    }
}
