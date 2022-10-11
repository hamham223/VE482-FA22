#ifndef COMMAND_H
#define COMMAND_H

#include "stdbool.h"

/**
 *@brief this file is for executing special commands and handling errors
 */


bool is_special(char * str);

void crtlC2();

void crtlC(int sig);

void mumsh_exit();

void mumsh_pwd();

void mumsh_prompt();

void open_error(char ** file);

void read_error(char ** file);

void mumsh_execvp(char ** start);

void error_prompt(int type);

bool is_duplicated_out(int f2, int f3, int * p);

#endif  // COMMAND_H
