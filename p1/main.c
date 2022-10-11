#include "parse.h"
#include "global.h"
#include "command.h"

const char _EXIT_[5] = "exit";
const char _IN_[2] = "<";
const char _OUT_[2] = ">";
const char _APPEND_[3] = ">>";
const char _PIPE_[2] = "|";
const char _CHDIR_[3] = "cd";
const char _PWD_[4] = "pwd";
const char _LAST_[2] = "-";
const char _HOME_[2] = "~";

void close_all(int f1, int f2, int f3, int p, int * fd){
    if (f1 >= 0) close(f1);
    if (f2 >= 0) close(f2);
    if (f3 >= 0) close(f3);
    if (p>0 && fd){
        if (fd[0] >= 0) {close(fd[0]);fd[0] = -1;}
        if (fd[1] >= 0) {close(fd[1]);fd[1] = -1;}
    }
}

void dup2_all(int f1, int f2, int f3){
    if (f1 >= 0) dup2(f1,STDIN_FILENO);
    if (f2 >= 0) dup2(f2,STDOUT_FILENO);
    if (f3 >= 0) dup2(f3,STDOUT_FILENO);
}

/**
 * @param special: 1 for cd, 2 for pwd, -1 for normal
 */ 
void forking(char ** start, int f1, int f2, int f3, int special){
    if (fork()==0){
        dup2_all(f1,f2,f3);
        switch (special) {
            case 2:
                mumsh_pwd();
                break;    
            default:
                mumsh_execvp(start);
                break;
        }
        exit(0);
    } else {
        wait(NULL);
    }
}

/**
 * @brief execute command, "cd" and "pwd" are build-in, handle "exit"
 */ 
void execute(char ** start,int f1,int f2,int f3){
    static char _LAST_DIR_ [MAX_CMD_LENGTH] = ".";
    static bool _IS_INITIAL = false;
    if (!_IS_INITIAL) {
        _IS_INITIAL = true;
        getcwd(_LAST_DIR_,1024);
    }
    if (!(*start)) {error_prompt(7); return;}
    char * first = start[0];
    int special = -1;
    if (strcmp(_EXIT_,first)==0) {
        delete_parse(start);
        close_all(f1,f2,f3,0,NULL);
        mumsh_exit();
    }
    if (strcmp(_CHDIR_,first)==0) {
        char tmp[MAX_CMD_LENGTH] = ".";
        getcwd(tmp,1024);
        if (start[1]==NULL) {
            start[1] = calloc(1,2);
            strcpy(start[1],_HOME_);
        }
        if (strcmp(start[1],_LAST_)==0){
            chdir(_LAST_DIR_);
            fprintf(stdout,"%s\n",_LAST_DIR_);
            strcpy(_LAST_DIR_,tmp);
        } else {
            if (strcmp(start[1],_HOME_)==0) {
                chdir(getenv("HOME"));
                strcpy(_LAST_DIR_,tmp);
            } else {
                if (chdir(start[1])==-1) {
                    perror(start[1]);
                } else {
                    strcpy(_LAST_DIR_,tmp);
                }
            }
        }
        return;
    } else {
        if (strcmp(_PWD_,first)==0){
            special = 2;
        }
    }
    signal(SIGINT,crtlC2);
    forking(start,f1,f2,f3,special);
}

void execute_pipe(char ** start, int * fd, int p){
    if (!(*start) || !(*(start+p+1))) {
        error_prompt(7);
        return;
    }
    signal(SIGINT,crtlC2);
    pid_t pid1 = fork();
    if (pid1 == 0) {
        dup2(fd[1],STDOUT_FILENO);
        close(fd[0]);
        mumsh_execvp(start);
        exit(0);
    } else {
        dup2(fd[0],STDIN_FILENO);
        close(fd[1]);
        mumsh_execvp(start+p+1);
        exit(0);
    }
}

void handle_file(char ** start){
    char ** pivot = start;
    int f1 = -1, f2 = -1, f3 = -1; // file I/O redirection
    int fd[2] = {-1, -1};          // pipe
    if (pipe(fd) == -1) {
        perror("Cannot create pipe.");
        _exit(1);
    }
    char ** arg_list = calloc(512,1);
    int count = 0; 
    int p = -1;
    while (*pivot){
        if (strcmp(_IN_,*pivot)==0) {
            if (f1 >= 0 || p >= 0) {
                error_prompt(2);
                p = -2; break;
            }
            if ((f1 = open(*(++pivot),O_RDONLY))< 0) read_error(pivot);
        } else {
            if (strcmp(_OUT_,*pivot)==0) {
                if (*(pivot+1) && is_special(*(pivot+1))) {
                    open_error(++pivot);
                } else {
                    if (is_duplicated_out(f2,f3,&p)) break;
                    if ((f2 = open(*(++pivot),O_WRONLY|O_CREAT|O_TRUNC,0600))< 0) open_error(pivot);
                }
            } else {
                if (strcmp(_APPEND_,*pivot)==0){
                    if (is_duplicated_out(f2,f3,&p)) break;
                    if ((f3 = open(*(++pivot),O_RDWR|O_CREAT|O_APPEND,0600))< 0) open_error(pivot);
                } else {
                    if (strcmp(_PIPE_,*pivot)==0){
                        if (p>=0 && p==count-1) { // two consecutive | |
                            p = -2;
                            error_prompt(7);
                            break;
                        }
                        if (is_duplicated_out(f2,f3,&p)) break;
                        p = count;
                        arg_list[count++] = NULL;
                    } else {
                        arg_list[count] = calloc(1,strlen(*pivot)+1);
                        strcpy(arg_list[count++],*pivot);
                    }
                }
            }
        }
        if (!(*pivot)) {p = -2; break;}
        pivot++;
    }
    delete_parse(start); 
    if (p == -1) {
        execute(arg_list,f1,f2,f3);
    } else {
        if (p != -2) {
            if (fork() == 0){
                execute_pipe(arg_list,fd,p);
                exit(0);
            } 
        }
    }
    close_all(f1,f2,f3,1,fd);
    wait(NULL);
    delete_parse(arg_list);
}

void mumsh_loop(){
    char ** start = NULL;
    while (true){
        signal(SIGINT,crtlC);
        char LINE[MAX_CMD_LENGTH];
        wait(NULL);
        mumsh_prompt();
        fgets(LINE, MAX_CMD_LENGTH, stdin);
        if (feof(stdin)) {
            mumsh_exit();
        }
        if (strlen(LINE) <= 1) continue; // only "\n"
        start = parse(LINE);
        handle_file(start);                          
    }
}

int main(int argc, char * argv[]){
    if (argc >= 2){
        if (strcmp("-v",argv[1])==0 || strcmp("--version",argv[1])==0){
            printf("mumsh v3.0\n");
            printf("Author: Hamster.!\n");
            return 0;
        }
        if (strcmp("-h",argv[1])==0 || strcmp("--help",argv[1])==0){
            printf("Haha, no help!!\n");
            return 0;
        }
    }
    mumsh_loop();
    return 0;
}
