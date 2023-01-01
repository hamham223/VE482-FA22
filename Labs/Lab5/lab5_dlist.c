#include "lab5_dlist.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#define MAX_VALUE_LEN 192
#define INT sizeof(int)
#define DOUBLE sizeof(double)

typedef struct _node_ {
    struct _node_ * next;
    dlistValue val;
    char * name;
} node_t;

typedef struct _list_{
    dlistValueType type;
    node_t * head;
} list_t;

dlist createDlist(dlistValueType _type_){
    list_t * list = malloc(sizeof(list_t));
    if (!list) return NULL;
    list->head = NULL;
    list->type = _type_;
    return list;
}

int dlistIsEmpty(dlist_const _this_){
    list_t * list = (list_t * )_this_;
    return (!(list) || !(list->head));
}

void dlistAppend(dlist _this_, const char* key, dlistValue value){
    list_t * list = (list_t *)_this_;
    node_t * new_head = malloc(sizeof(node_t));
    new_head->name = malloc(strlen(key)+1);
    strcpy(new_head->name,key);
    if (list->type!=DLIST_STR) new_head->val = value;
    else {
        new_head->val.strValue = malloc(strlen(value.strValue)+1);
        strcpy(new_head->val.strValue,value.strValue);
    }
    new_head->next = list->head;
    list->head = new_head;
}

static void nodePrint(node_t * cur, dlistValueType _type_){
    //fprintf(stderr,"nodePrint called!\n");
    fprintf(stdout,"%s=",cur->name);
    if (_type_==1) fprintf(stdout,"%d\n",cur->val.intValue);
    if (_type_==3) fprintf(stdout,"%f\n",cur->val.doubleValue);
    if (_type_==2) fprintf(stdout,"%s\n",cur->val.strValue);
    if (_type_==0) fprintf(stdout,"\n");
}

void dlistPrint(dlist_const _this_){
    list_t * list = (list_t *)_this_;
    node_t * cur = list->head;
    while (cur) {
        nodePrint(cur,list->type);
        cur = cur->next;
    }
}

static void nodeFree(node_t * node){
    free(node->name);
    free(node);
}

void dlistFree(dlist _this_){
    list_t * list = (list_t *)_this_;
    node_t * cur = list->head;
    while (cur) { // empty, do nothing
        node_t * tmp = cur;
        cur = cur->next;
        if (list->type==DLIST_STR) free(tmp->val.strValue);
        nodeFree(tmp);
    }
    free(list);
}

static void copy_shallow(node_t ** head, node_t ** node){
    node_t * tmp = *head;
    node_t * tmp2 = (*node)->next;
    *head = *node;
    (*head)->next = tmp;
    *node = tmp2;
}

/**
 * @return true if left is strictly less than right 
 */
static bool less(node_t * left, node_t * right, int type){
    dlistValue l = left->val;
    dlistValue r = right->val;
    if (type==1) return (l.intValue<r.intValue);
    if (type==3) return (l.doubleValue<r.doubleValue);
    return (strcmp(l.strValue,r.strValue)<0);
}

static bool greater(node_t * left, node_t * right, int type){
    dlistValue l = left->val;
    dlistValue r = right->val;
    if (type==1) return (l.intValue>r.intValue);
    if (type==3) return (l.doubleValue>r.doubleValue);
    return (strcmp(l.strValue,r.strValue)>0);
}

static bool Ordless(node_t * left, node_t * right, int type, int ord) {
    if (ord==3) return greater(left,right,type);
    if (ord==2) return less(left,right,type);
    return rand()%2;
}

static node_t * reverse(node_t * head){
    if (!head | !(head->next)) return head;
    node_t * first = head;
    node_t * second = head->next;
    node_t * third = second->next;
    head->next = NULL;
    while (third){
        second->next = first;
        first = second;
        second = third;
        third = third->next;
    }
    second->next = first;
    return second;
}

/**
 * @param head: head of the first list
 * @param mid: head of the second list
 * @brief use an extra list to merge 2 lists, free origin one
 */
static node_t * merge(node_t * head, node_t * mid, int type, int ord){
    node_t * new_head = NULL;
    while (head && mid) {
        if (Ordless(head,mid,type,ord)) {
            copy_shallow(&new_head,&head);
        } else {
            copy_shallow(&new_head,&mid);
        }
    }
    while (head) {
        copy_shallow(&new_head,&head);
    }
    while (mid){
        copy_shallow(&new_head,&mid);
    }
    //return new_head;
    return reverse(new_head);
}

/**
 * @brief get the mid point of the list
 * @brief , head MUST NOT be NULL
 */
static node_t * getMid(node_t * head){
    node_t * end = head->next;
    while (end){
        end = end->next;
        if (end) end = end->next;
        else break;
        head = head->next;
    }
    return head;
}

static node_t * mergesort(node_t * head, int type, int ord){
    if (!head || !(head->next)) return head; // 0 or 1 element(s)
    node_t * mid = getMid(head);
    node_t * tmp = mid->next;
    mid->next = NULL; // break the list
    head = mergesort(head,type,ord);
    tmp = mergesort(tmp,type,ord);
    return merge(head,tmp,type,ord);
}

node_t * DeepCopyHelper(node_t * node,int ischar){
    if (!node) return NULL;
    node_t * cur = malloc(sizeof(node_t));
    cur->next = DeepCopyHelper(node->next,ischar);
    cur->name = malloc(strlen(node->name)+1);
    strcpy(cur->name,node->name);
    if (!ischar) cur->val = node->val;
    else {
        cur->val.strValue = malloc(strlen(node->val.strValue)+1);
        strcpy(cur->val.strValue,node->val.strValue);
    }
    return cur;
}

/**
 *@brief deep copy from list to res
 *@param res, must be already created;
*/
void dlistDeepCopy(list_t * res, list_t * list){
    node_t * cur = res->head;
    while (cur) {
        node_t * tmp = cur;
        cur = cur->next;
        if (res->type==DLIST_STR) free(tmp->val.strValue);
        nodeFree(tmp);
    }
    res->head = DeepCopyHelper(list->head,(list->type==DLIST_STR));
}

void dlistSort(dlist_const _this_, dlist listDst, dlistSortMethod method){
    list_t * list = (list_t *)_this_;
    if (!list->head) return;
    list_t * res = (list_t *) listDst;
    if (res->head) {
        fprintf(stderr,"Already sort once!\n");
        return;
    }
    list->head = mergesort(list->head,(int)list->type,(int)method);
    dlistDeepCopy((list_t *)listDst,list);
}
