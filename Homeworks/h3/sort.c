#include "sort.h"

bool less_int(int * a, int * b) { return (*a<*b?true:false);}
bool less_double(double * a, double * b) { return (*a<*b?true:false);}
bool less_char(char * a, char * b) {return (strcmp(a,b)<0?true:false);}
bool greater_int(int * a, int * b) { return (*a>*b?true:false);}
bool greater_double(double * a, double * b) { return (*a>*b?true:false);}
bool greater_char(char * a, char * b) {return (strcmp(a,b)>0?true:false);}

/**
 * @return true if left is strictly less than right 
 */
bool less(node_t * left, node_t * right, int type){
    void * l = left->elem; void * r = right->elem;
    if (type==1) return less_int(l,r);
    if (type==2) return less_double(l,r);
    return less_char(l,r);
}

bool greater(node_t * left, node_t * right, int type){
    void * l = left->elem; void * r = right->elem;
    if (type==1) return greater_int(l,r);
    if (type==2) return greater_double(l,r);
    return greater_char(l,r);
}

bool Ordless(node_t * left, node_t * right, int type, int ord) {
    if (ord==2) return greater(left,right,type);
    if (ord==1) return less(left,right,type);
    return rand()%2;
}



node_t * reverse(node_t * head){
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
node_t * merge(node_t * head, node_t * mid, int type, int ord){
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
node_t * getMid(node_t * head){
    node_t * end = head->next;
    while (end){
        end = end->next;
        if (end) end = end->next;
        else break;
        head = head->next;
    }
    //fprintf(stdout,"Mg %c ",head->name[0]);
    //fflush(stdout);
    return head;
}

node_t *  mergesort(node_t * head, int type, int ord){
    if (!head || !(head->next)) return head; // 0 or 1 element(s)
    //fprintf(stdout,"To get mid point %s ",head->name);
    //fflush(stdout);
    node_t * mid = getMid(head);
    node_t * tmp = mid->next;
    mid->next = NULL; // break the list
    head = mergesort(head,type,ord);
    tmp = mergesort(tmp,type,ord);
    //fprintf(stdout,"M %c %c ",head->name[0],tmp->name[0]);
    //fflush(stdout);
    return merge(head,tmp,type,ord);
}

