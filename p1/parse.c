#include "parse.h"
#include "global.h"

bool is_redirect(char c){
    return (c=='<' || c=='>');
}

bool is_append(char * start){
    return (strcmp(start,">>")==0);
}

/**
 *@return pos of next redirection symbol, return strlen() if not found 
 */
unsigned int find_next_redriect(char* token){
    unsigned int i = 0;
    for (; i < strlen(token) && !is_redirect(token[i]); i++);
    return i;
}

/**
 * @brief add arg in to the 2D array 
 */
void add_to_arg(char ** result, int * count, char * arg){
    result[*count] = calloc(1,strlen(arg)+1);
    strcpy(result[*count],arg);
    *count = * count + 1;
}

/**
 * @brief parse redirection symbol "<", ">" and ">>" 
 * @param token: the command to be parsed, e.g. "<a.txt>b.txt"
 * @param result: array to store the result, "<", "a.txt", ">", "b.txt"
 * @param count: pointer to the array pivot
 */
void parse_redirect(char * token,char ** result, int * count){
    if (!token) return;
    unsigned int pivot = find_next_redriect(token);
    if (pivot==0){ // start with > or >> or <
        if (*token=='>' && *(token+1)=='>') {
            add_to_arg(result,count,">>");
            if (strlen(token)==2) return;
            token++;
        } else {
            char tmp[2];
            sprintf(tmp,"%c",*token);
            add_to_arg(result,count,tmp);
        }
        token++;
    } else {
        char * tmp = calloc(1,pivot+1);
        strncpy(tmp,token,pivot);
        add_to_arg(result,count,tmp);
        free(tmp);
        if (pivot < strlen(token)) token = token + pivot;
        else token = NULL;
    }
    parse_redirect(token,result,count);
}


/**
 *@param LINE: pointer to the input line
 *@return Dynamic array of parsed word, end with "" 
 */
char ** parse(char * LINE){
    int count = 0; // size
    LINE[strlen(LINE)-1]='\0';
    char * token = strtok(LINE, " ");
    char ** result = calloc(513,1);
    while (token){
        if (strlen(token)>1) { 
            parse_redirect(token,result,&count);
        } else {                              
            add_to_arg(result,&count,token);
        }
        token = strtok(NULL, " ");
    }
    result[count]=NULL;
    return result;
}

void delete_helper(char ** start){
    if (!start) return;
    int i = 0;
    for (;start[i];i++) free(start[i]);
    if (start[i+1]) delete_helper(start+i+1);
}

/**
 * @brief free 2D dynamic memory
 * 
 */ 
void delete_parse(char ** start){
    delete_helper(start);
    free(start);
}
