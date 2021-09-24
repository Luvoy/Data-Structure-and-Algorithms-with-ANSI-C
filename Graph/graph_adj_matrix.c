#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <malloc.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

typedef void *data_t;
typedef unsigned int weight_t;
#define INFINITE_WEIGHT -1
typedef int index_t; //索引或数量类型，本来想用size_t，其实负数反而也有用
typedef int status_t;
typedef struct graph
{
    index_t vertex_num;
    index_t edge_num;
    data_t *vertex_data_array;
    weight_t **adj_matrix;
} Graph;

#ifndef DEBUG_DYNAMIC_MEM_CHECK
#define DEBUG_DYNAMIC_MEM_CHECK
#endif
#ifdef DEBUG_DYNAMIC_MEM_CHECK
int g_alloc_count = 0;
int g_free_count = 0;
void my_free(void *p)
{
    free(p);
#ifdef DEBUG_DYNAMIC_MEM_CHECK
    g_free_count++;
#endif
}
#endif

#ifndef MY_DATA_TYPE
#define MY_DATA_TYPE
#endif
#ifdef MY_DATA_TYPE
#define MAX_STR_LEN 128
void fill_data(data_t *p, FILE *f)
{ //Overwrite
    *p = (char *)malloc(MAX_STR_LEN * sizeof(char));
#ifdef DEBUG_DYNAMIC_MEM_CHECK
    g_alloc_count++;
#endif
    fscanf(f, "%s\n", (char *)(*p));
}
void visit_data(const data_t *p, const char *format)
{ //Overwrite
    printf(format, (const char *)(*p));
}
void free_data(data_t *p)
{ //c语言没有面向对象，拷贝析构都是问题
    if ((char *)(*p) != NULL)
    {
        my_free((char *)(*p));
        *p = NULL; //这并不能阻止2次释放！ 因为改变的是传过来的左值。而data那个地址还在那里，第二次释放时，并不知道这个地址已经释放了
    }
}
#endif
void fill_graph(Graph *gp, FILE *f)
{
    index_t i, v_start, v_end;
    weight_t w;
    for (i = 0; i < gp->vertex_num; ++i)
    {
        fill_data(gp->vertex_data_array + i, f);
    }
    for (i = 0; i < gp->edge_num; ++i)
    {
        fscanf(f, "%d %d %u ", &v_start, &v_end, &w);
        (gp->adj_matrix)[v_start][v_end] = w;
        //注意，这里相当于规定了行是出，列是入。
        //For example, matrix[2][3] denotes the edge from the 3rd vertex to the 4th, rather than the other way around.
    }
}

Graph *create_graph_raw(index_t vertex_num, index_t edge_num)
{ //创建空图
    index_t i;

    Graph *ret = (Graph *)malloc(sizeof(Graph));
#ifdef DEBUG_DYNAMIC_MEM_CHECK
    g_alloc_count++;
#endif

    ret->vertex_num = vertex_num;
    ret->edge_num = edge_num;

    ret->vertex_data_array = (data_t *)calloc(sizeof(data_t), vertex_num);
#ifdef DEBUG_DYNAMIC_MEM_CHECK
    g_alloc_count++;
#endif

    ret->adj_matrix = (weight_t **)malloc(vertex_num * sizeof(weight_t *));
#ifdef DEBUG_DYNAMIC_MEM_CHECK
    g_alloc_count++;
#endif

    for (i = 0; i < vertex_num; ++i)
    {
        (ret->adj_matrix)[i] = (weight_t *)malloc(vertex_num * sizeof(weight_t));
#ifdef DEBUG_DYNAMIC_MEM_CHECK
        g_alloc_count++;
#endif
        memset(ret->adj_matrix[i], INFINITE_WEIGHT, vertex_num * sizeof(weight_t)); //初始都设成无穷

        (ret->adj_matrix)[i][i] = 0;
    }

    return ret;
}

Graph *min_plant_Prim(Graph *gp, int bool_directed)
{ //assume caller free
    Graph *ret = create_graph_raw(gp->vertex_num, gp->edge_num);
    memcpy(ret->vertex_data_array, gp->vertex_data_array, sizeof(data_t) * gp->vertex_num); //copy

    weight_t lowcost[gp->vertex_num],
        min_cost;
    index_t teend[gp->vertex_num], i, j, k;
    //lowcost[i]表示：teend[i]与i的边长，即看当前生成树已有各个节点到i的距离，哪个已有节点到i的距离最小？teend[i]就是这个已有节点，lowcost[i]就是这个距离
    lowcost[0] = 0; //v0已经加入生成树
    teend[0] = 0;
    for (i = 1; i < gp->vertex_num; i++)
    {
        teend[i] = 0;
        lowcost[i] = gp->adj_matrix[0][i]; //未加入的各个点到v0的距离，有些可能是无穷
    }

    for (i = 1; i < gp->vertex_num; ++i)
    {
        min_cost = INFINITE_WEIGHT;
        for (j = 1; j < gp->vertex_num; ++j)
        {
            if (lowcost[j] > 0 && lowcost[j] < min_cost)
            {
                min_cost = lowcost[j];
                k = j;
            }
        } //选了一条当前点（已加入生成树）的边（想象向外延申的感觉）中最短的边，并记录下了来源点（也就是还没加入生成树那个点）的下标

        ret->adj_matrix[teend[k]][k] = gp->adj_matrix[teend[k]][k]; //输出这个边
        if (!bool_directed)
            ret->adj_matrix[k][teend[k]] = gp->adj_matrix[teend[k]][k]; //无向图对称

        lowcost[k] = 0; //清零表示这个点已经加入生成树了

        for (j = 0; j < gp->vertex_num; ++j)
        { //更新lowcost
            if (gp->adj_matrix[k][j] < lowcost[j])
            { //用新加入的点延伸爪子，即看新加入点的各个边有没有更小的，加进去，并记下来这个边是这个新点，和新点的终点弄来的
                lowcost[j] = gp->adj_matrix[k][j];
                teend[j] = k;
            }
        }

        //巧妙之处在于，n个节点的最小生成树必然n-1个边，不多不少，每加入一个点，还没找到的边就少一个，i个已加入点，就剩n-1-i个边没找到，而一个点最多有n-1条边，lowcost[n]正好把已经加入的点设为0，（最开始是自己），所以每次遍历新加入节点所带来的延申边，就可以更新生成树所有延申边，进而求生成树所有延申边中最短的。其实当前生成树的所有延申边的长度，就是lowcost数组，只不过自己人设成0
    }

    return ret;
}

typedef struct edge
{ //为Kruskal算法准备的数据结构，独立存放一条边的信息
    //因为cmp函数要给出具体指针类型，所以必须定义在函数之外了
    index_t v_start;
    index_t v_end;
    weight_t w;
} edge_t;
int cmp(edge_t *e1, edge_t *e2) { return (e1->w) - (e2->w); } //为sort做准备
Graph *min_plant_Kruskal(Graph *gp, int bool_directed)
{ //assume caller free
    Graph *ret = create_graph_raw(gp->vertex_num, gp->edge_num);
    memcpy(ret->vertex_data_array, gp->vertex_data_array, sizeof(data_t) * gp->vertex_num); //copy

    edge_t e[gp->edge_num];
    index_t v_terminals[gp->vertex_num]; //用来保存每个节点在已有最小树的终点，如果一条边的两个点的终点相同，说明有回路
    index_t i, j, k = 0;
    for (i = 0; i < gp->vertex_num; ++i)
    {
        v_terminals[i] = -1; //顺便赋个初值
        for (j = 0; j < gp->vertex_num; ++j)
        {
            if (i != j && gp->adj_matrix[i][j] != INFINITE_WEIGHT)
            {
                e[k].v_start = i;
                e[k].v_end = j;
                e[k++].w = gp->adj_matrix[i][j];
            }
        }
    }
    qsort(e, gp->edge_num, sizeof(edge_t), cmp); //stdlib.h

    index_t start_ter, end_ter, found_count = 0;
    for (i = 0; i < gp->edge_num; ++i)
    {
        start_ter = e[i].v_start;
        end_ter = e[i].v_end;

        while (-1 != v_terminals[start_ter])
            start_ter = v_terminals[start_ter]; //静态链表法

        while (-1 != v_terminals[end_ter])
            end_ter = v_terminals[end_ter]; //静态链表法

        if (start_ter != end_ter)
        { //没有环路
            v_terminals[start_ter] = end_ter;
            ret->adj_matrix[e[i].v_start][e[i].v_end] = e[i].w;
            if (!bool_directed) //无向图对称补充
                ret->adj_matrix[e[i].v_end][e[i].v_start] = e[i].w;
            if (++found_count >= gp->vertex_num - 1)
                break; //找到n-1个即可结束
        }
        if (!bool_directed)
            ++i; //无向图，对称阵，边成对存在，每次增2即可
    }
    return ret;
}

void destroy_graph(Graph *gp)
{
    index_t i;
    for (i = 0; i < gp->vertex_num; ++i)
    {
        my_free((gp->adj_matrix)[i]);
        free_data(gp->vertex_data_array + i);
    }
    my_free(gp->adj_matrix);
    my_free(gp->vertex_data_array);
    my_free(gp);
}

void shortest_paths_Dijkstra(Graph *gp, index_t v, index_t (*paths)[gp->vertex_num])
{
    if (v < 0 || v >= gp->vertex_num)
        return;

    weight_t accumu_dist[gp->vertex_num]; //所求点v到各个点的累计最短路径长度和
    int found_flag[gp->vertex_num];       //各个点是否已经找到了路径
    int pos[gp->vertex_num];              //各个点的路径的计数器, 巧妙而别扭
    index_t i, j, k;

    memset(paths, -1, gp->vertex_num * gp->vertex_num * sizeof(index_t)); //paths二维数组必然地址连续
    for (i = 0; i < gp->vertex_num; ++i)
    { //辅助数组初始化
        found_flag[i] = 0;
        accumu_dist[i] = gp->adj_matrix[v][i]; //初始时, 等于matrix的第v行, 能直接到的就一条边的事情, 没有直接一条边的相当于infinite
        paths[i][0] = v;                       //初始时,到达各个点的路径上,只有v自己
        pos[i] = 0;
    }
    found_flag[v] = 1; //显然自己到自己找到了
    for (i = 1; i < gp->vertex_num; ++i)
    { //重复n-1次这样的过程
        index_t min_v = -1;
        weight_t min_w = INFINITE_WEIGHT;
        for (j = 0; j < gp->vertex_num; ++j)
        { //在剩下的尚未找到路径的节点中找最短的
            if (j != v && accumu_dist[j] < min_w && found_flag[j] == 0)
            {
                min_w = accumu_dist[j]; //记录最短距离值
                min_v = j;              //记录这个距离所来自的节点
            }
        }
        if (min_w == INFINITE_WEIGHT)
            continue; //not found说明不连通
        else
        {
            found_flag[min_v] = 1;
            paths[min_v][++pos[min_v]] = min_v; //即paths[找到的节点所在行][下一列]
        }

        //接下来根据min_v更新所有的 尚未确定路径的顶点 的长度
        for (j = 0; j < gp->vertex_num; j++)
        { //有没有通过刚才min_v可以直接到达,且尚未确定路径的节点？
            if (j != min_v && found_flag[j] == 0 && gp->adj_matrix[min_v][j] < INFINITE_WEIGHT)
            {
                if (accumu_dist[min_v] + gp->adj_matrix[min_v][j] < accumu_dist[j])
                { //经过min_v到j比由v直接到j确实短,才更新
                    accumu_dist[j] = accumu_dist[min_v] + gp->adj_matrix[min_v][j];
                    pos[j] = pos[min_v];
                    for (k = 0; k <= pos[min_v]; ++k) //v到j的路径, 用刚才min_v的去替换, 即v到j的前几步,必然是通过min_v过去的
                        paths[j][k] = paths[min_v][k];
                }
            }
        }
    }
}

void print_naive(Graph *gp, void (*visit)(const data_t *, const char *), size_t col_interval);
int main(int argc, char const *argv[])
{

    index_t i, j;
    freopen("in.txt", "r", stdin);
    Graph *g1 = create_graph_raw(6, 20);
    fill_graph(g1, stdin);

    print_naive(g1, visit_data, 2);

    printf("***************************************\n");
    Graph *g2 = min_plant_Prim(g1, 0);
    print_naive(g2, visit_data, 2);

    printf("***************************************\n");

    Graph *g3 = min_plant_Kruskal(g1, 0);
    print_naive(g3, visit_data, 2);

    destroy_graph(g1);
    for (i = 0; i < g2->vertex_num; ++i) //权宜之计
        g2->vertex_data_array[i] = NULL;
    destroy_graph(g2);
    for (i = 0; i < g3->vertex_num; ++i)
        g3->vertex_data_array[i] = NULL;
    destroy_graph(g3);
    printf("***************************************\n");
    Graph *g4 = create_graph_raw(7, 18);
    fill_graph(g4, stdin);

    print_naive(g4, visit_data, 1);

    index_t paths[7][7];

    shortest_paths_Dijkstra(g4, 0, paths);

    for (i = 0; i < 7; ++i)
    {
        for (j = 0; j < 7; ++j)
            printf("%5d", paths[i][j]);
        printf("\n");
    }

    destroy_graph(g4);

    printf("***************************************\n");
    freopen("in2.txt", "r", stdin);
    Graph *g5 = create_graph_raw(5, 7);
    fill_graph(g5, stdin);

    print_naive(g5, visit_data, 1);

    printf("***************************************\n");
    Graph *g6 = min_plant_Prim(g5, 1);
    print_naive(g6, visit_data, 1);
    printf("***************************************\n");

    //经测试，有向图的Prim和Krustal算法结果不一样。Prim是严格有向（即1个顶点入度0，其余顶点入度1）的最小生成树树，Krustal仅保证结果有无回路的n-1条边且权值和最小
    Graph *g7 = min_plant_Kruskal(g5, 1);
    print_naive(g7, visit_data, 1);

    printf("***************************************\n");
    index_t paths2[5][5];

    shortest_paths_Dijkstra(g5, 0, paths2);

    for (i = 0; i < 5; ++i)
    {
        for (j = 0; j < 5; ++j)
            printf("%5d", paths2[i][j]);
        printf("\n");
    }
    destroy_graph(g5);
    for (i = 0; i < g6->vertex_num; ++i)
        g6->vertex_data_array[i] = NULL;
    destroy_graph(g6);
    for (i = 0; i < g7->vertex_num; ++i)
        g7->vertex_data_array[i] = NULL;
    destroy_graph(g7);
#ifdef DEBUG_DYNAMIC_MEM_CHECK
    fprintf(stderr, "g_alloc_count: %d, g_free_count: %d.\n", g_alloc_count, g_free_count);
#endif

    return 0;
}

//////////////////////////////////////////////////////
#define max(a, b) ((a) > (b) ? (a) : (b))
void print_naive(Graph *gp, void (*visit)(const data_t *, const char *), size_t col_interval)
{ //朴素打印（对齐）
    index_t i, j;
    const char *infinite = "INF";
    size_t col_len[gp->vertex_num];
    for (i = 0; i < gp->vertex_num; ++i)
        col_len[i] = 0;
    size_t w_2_str_max_len = 0;
    size_t d_2_str_max_len = 0;
    size_t all_str_max_len = 0;
    weight_t w = UINT_MAX;
    while (w)
    {
        w /= 10;
        w_2_str_max_len++;
    }
    w_2_str_max_len = max(strlen(infinite), w_2_str_max_len); //for infinite
    char w_2_str[w_2_str_max_len];
    for (i = 0; i < gp->vertex_num; ++i)
    {
        for (j = 0; j < gp->vertex_num; ++j)
        {
            if (gp->adj_matrix[i][j] == INFINITE_WEIGHT)
                col_len[i] = max(col_len[i], strlen(infinite));
            else
            {
                sprintf(w_2_str, "%u", gp->adj_matrix[i][j]);
                col_len[i] = max(col_len[i], strlen(w_2_str));
            }
        }
        col_len[i] = max(col_len[i], strlen(gp->vertex_data_array[i]));
        d_2_str_max_len = max(d_2_str_max_len, strlen(gp->vertex_data_array[i]));
    }

    for (i = 0; i < gp->vertex_num; ++i)
        all_str_max_len = max(all_str_max_len, col_len[i]);
    for (i = 0; i < d_2_str_max_len + col_interval; ++i)
        printf(" ");
    char format[all_str_max_len + 2];
    for (i = 0; i < gp->vertex_num; ++i)
    {
        memset(format, (int)('\0'), all_str_max_len + 2);
        // sprintf(format, "%%%us\0", col_len[i] + col_interval); //错误在于,你把\0放在""里面写,就不是真正的\0了
        sprintf(format, "%%%us", col_len[i] + col_interval);
        visit(gp->vertex_data_array + i, format);
    }
    printf("\n");
    char format_1st_col[d_2_str_max_len + 2];
    memset(format_1st_col, (int)('\0'), d_2_str_max_len + 2);
    sprintf(format_1st_col, "%%%us", d_2_str_max_len + col_interval);
    for (i = 0; i < gp->vertex_num; ++i)
    {
        visit(gp->vertex_data_array + i, format_1st_col);
        for (j = 0; j < gp->vertex_num; ++j)
        {
            memset(format, (int)('\0'), all_str_max_len + 2);
            if (gp->adj_matrix[i][j] == INFINITE_WEIGHT)
            {
                sprintf(format, "%%%us", col_len[j] + col_interval);
                printf(format, infinite);
            }
            else
            {
                sprintf(format, "%%%uu", col_len[j] + col_interval);
                printf(format, gp->adj_matrix[i][j]);
            }
        }
        printf("\n");
    }
}
