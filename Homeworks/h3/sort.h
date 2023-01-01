#ifndef SORT_H
#define SORT_H

#include "list.h"

bool less(node_t * left, node_t * right, int type);
bool Ordless(node_t * left, node_t * right, int type, int ord);

node_t * getMid(node_t * head);

node_t *  mergesort(node_t * head, int type, int ord);
node_t * merge(node_t * head, node_t * mid, int type, int ord);

#endif 
