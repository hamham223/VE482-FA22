#include "list.h"
const unsigned int _LENGTH_[3] = {(unsigned)168,sizeof(int),sizeof(double)};


void delete_list(node_t * head){
    while (head) head = delete_head(head);
}

bool isEqual(int type, void * a, void * b) {
    if (type==1 && *((int*)a)==*((int*)b)) return true;
    if (type==2 && *((double*)a)==*((double*)b)) return true;
    return (type==0 && strcmp((char*)a,(char*)b)==0);
}

/**
 * @return true if find the value
 */
bool search_list(node_t * head, int type, void * val){
    if (!head) return false;
    if (isEqual(type,head->elem,val)) return true;
    else return search_list(head->next,type,val);
}

node_t * insert_node(node_t * head, void * val, char * name){
    node_t * new_head = malloc(sizeof(node_t));
    new_head->elem = val;
    new_head->name = name;
    new_head->next = head;
    return new_head;
}

void copy_shallow(node_t ** head, node_t ** node){
    node_t * tmp = *head;
    node_t * tmp2 = (*node)->next;
    *head = *node;
    (*head)->next = tmp;
    *node = tmp2;
}

node_t * copy_node(node_t * head, node_t * node,int type){
    node_t * new_head = malloc(sizeof(node_t));
    void * val = malloc(_LENGTH_[type]);
    char * name = malloc(strlen(node->name)+2);
    memcpy(val,node->elem,_LENGTH_[type]);
    strncpy(name,node->name,strlen(node->name)+2);
    new_head->elem = val;
    new_head->name = name;
    new_head->next = head;
    //node_t * new_head = node;
    //new_head->next = head;//type;
    return new_head;
}

/**
 * @return pointer to new head
 */
node_t* delete_head(node_t * head){
    if (!head) return NULL;
    node_t * new_head = head->next;
    free(head->elem);
    free(head->name);
    free(head);
    return new_head;
}

void printList(node_t * head,int type) {
    if (!head) return;
    printf("%s=",head->name);
    if (type==1) printf("%d\n",*((int*)head->elem));
    if (type==2) printf("%lf\n",*((double*)head->elem));
    if (type==0) printf("%s\n",(char*)head->elem);
    printList(head->next,type);
}

void fprintList(node_t * head,int type, FILE * f) {
    if (!head) return;
    fprintf(f,"%s=",head->name);
    if (type==1) fprintf(f,"%d\n",*((int*)head->elem));
    if (type==2) fprintf(f,"%lf\n",*((double*)head->elem));
    if (type==0) fprintf(f,"%s\n",(char*)head->elem);
    fprintList(head->next,type,f);
}
