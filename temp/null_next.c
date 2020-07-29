/* 尝试访问NULL->next*/
#include <stdio.h>
typedef struct Node
{
    struct Node *next;
    int data;
} Node;
int main(int argc, char const *argv[])
{
    Node a = {NULL,
              0};
    Node *p = a.next;
    while (p->next)
    {
        printf("haha\n");
    }
    return 0;
}
