#include "command.h"
#include "global.h"

bool is_special(char * str){
    if (strcmp(str,"|")==0) return true;
    if (strcmp(str,">")==0) return true;
    if (strcmp(str,"<")==0) return true;
    if (strcmp(str,">>")==0) return true;
    return false;
}

void mumsh_prompt(){
    printf("mumsh $ ");
    fflush(stdout);
}

void crtlC2(){}

void crtlC(int sig){
    sig += 1;
    if (getpid()!=0) {
        //printf(" Caught crtl+C in pid = %d and parent pid = %d\n",getpid(),getppid());
        mumsh_prompt();
    }
    //signal(sig,handle_sig2);
}

/**
 *@brief EVERYTHING need to be cleaned up before call this function!
 */
void mumsh_exit(){
    puts("exit");
    exit(0);
}

void mumsh_pwd(){
    char * buf = getcwd(NULL,0);
    printf("%s\n",buf); 
    fflush(stdout);
    free(buf);
}

void read_error(char ** file) {
    printf("%s: No such file or directory\n",*file); 
    free(*file);  
    *file = NULL;      
    //_exit(-errno);
}

void open_error(char ** file) {
    if (*file && is_special(*file)) {
        fprintf(stderr,"syntax error near unexpected token `%s'\n",*file);
    } else {
        fprintf(stderr,"%s: Permission denied\n",*file);
    }
    if (*file) free(*file);
    *file = NULL;  
    //_exit(-errno);
}

void error_prompt(int error_type){
    switch (error_type){
        case 1:
            perror(": command not found\n");
            break;
        case 3:
            fprintf(stderr,"error: duplicated output redirection\n");
            break;
        case 2:
            fprintf(stderr,"error: duplicated input redirection\n");
            break;
        case 7:
            fprintf(stderr,"error: missing program\n");
            break;
        default:
            perror("unknown error!\n");
            break;
    }
}


void mumsh_execvp(char ** start){
    if (execvp(start[0],start)<0) 
        printf("%s: command not found\n",start[0]);
    exit(0);
}

bool is_duplicated_out(int f2, int f3, int * p){
    if (f2 >=0 || f3 >= 0) {
        *p = -2;
        error_prompt(3);
        return true;
    }
    return false;
}
