#ifndef __STR_H__
#define __STR_H__




typedef struct graph {
int nr_nodes;
struct node** neighbours;
} graph;

typedef struct node {
int val, size;
char name[100];
struct node* next, *head, *tail;
} node;

#endif
