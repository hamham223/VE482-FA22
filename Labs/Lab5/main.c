#include "lab5_dlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

dlistValueType getValueType(char * file){
  char * pos = strchr(file,'_'); 
  if (*(pos+1)=='i') return DLIST_INT;
  if (*(pos+1)=='d') return DLIST_DOUBLE;
  if (*(pos+1)=='c') return DLIST_STR;
  return DLIST_UNKOWN;
}

dlistSortMethod getSortMethod(char * ord){
  if (ord[0]=='i') return DLIST_SORT_INC;
  if (ord[0]=='d') return DLIST_SORT_DEC;
  if (ord[0]=='r') return DLIST_SORT_RAND;
  return DLIST_SORT_UNKOWN;
}

void getKeyFromLine(char * key, char * line){
  char * pos = strchr(line,'=');
  *pos = '\0';
  strcpy(key,line);
  *pos = '=';
}

void getValueFromLine(dlistValue* value,char* line,dlistValueType type){
  size_t len = strlen(line);
  if (line[len-1]=='\n') line[--len]='\0';
  char * pos = strchr(line,'=');
  pos++;
  if (type==DLIST_INT) value->intValue= atoi(pos);
  if (type==DLIST_DOUBLE) value->doubleValue = atof(pos);
  if (type==DLIST_STR) {
    value->strValue = malloc(strlen(pos)+1);
    strcpy(value->strValue,pos);
  }
}

int run(char *fileName, char *order) {
  FILE *fp = fopen(fileName, "r");
  if (fp == NULL) exit(EXIT_FAILURE);
  dlistValueType type = getValueType(fileName);
  dlistSortMethod method = getSortMethod(order);
  if (type == DLIST_UNKOWN || method == DLIST_SORT_UNKOWN) exit(EXIT_FAILURE);
  dlist listDst = createDlist(type), list = createDlist(type);
  char line[2048], key[2048];
  dlistValue value;
  while (fgets(line, sizeof(line), fp)) {
    getKeyFromLine(key, line);
    getValueFromLine(&value, line, type);
    dlistAppend(list, key, value);
  }
  fclose(fp);
  dlistSort(list, listDst, method);
  dlistSort(list, listDst, method);
  dlistPrint(listDst);
  dlistFree(list);
  dlistFree(listDst);
  return 0;
}

int main(int argc, char *argv[]) {
  if (argc < 3) return -1;
  run(argv[1], argv[2]);
  return 0;
}
