#ifndef LIST_H
#define LIST_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>


#define _NAME_LEN  1024

typedef struct _node { 
    void * elem;  
    char * name;
    //int type; // 1 for int, 2 for double, 3 for char *, -1 not init
    struct _node * next;
} node_t;

int getOrd(char * token);
int getType(char * token);

node_t * insert_node(node_t * head, void * val, char * name);
node_t * copy_node(node_t * head, node_t * node,int type);
void copy_shallow(node_t ** head, node_t ** node);

node_t* delete_head(node_t * head);

void delete_list(node_t * head);

bool search_list(node_t * head, int type, void * val);

void printList(node_t * head, int type);
void fprintList(node_t * head,int type, FILE * f);

#endif
