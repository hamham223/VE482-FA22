## ECE4820J Lab 6

> Author: :hamster: 

## About Plugin

### How to create Plugin in C

> https://nachtimwald.com/2020/05/12/making-and-using-c-plugins/#:~:text=Making%20and%20Using%20C%20plugins%201%20The%20Plugins,example%20plugins.%20...%205%20The%20test%20app%20

### Why we need plugin (motivation)

+ Some users want more features for an app
+ Some users want the app to be small
+ plugin allows users to diy the features for the app
+ plugin can be powerful (cross - programming languages)

### What to do to create a plugin

+ an API that plugins will implement
+ a second API of function that will be exposed by the app to the plugins

#### Required Functions

The `init` and `deinit` functions are optional and will allow plugins to initialize themselves in some way.

The `name`, `type`, `version` and `api_version` functions are all required. Without these we won’t be able to reference, or know how to use, a given plugin.

#### Plugin Manager

A plugin manager handles loading, unloading, initialization, deinitialization for plugins.

## Basic Skeleton

```c++
#include ...

#define TXT_FILE 1
#define CVS_FILE 2
#define UNKNOWN 0

/**
 *@param extention, denote the file extention
 *@return int, the type of the file
 */
int getType(char *extention){
	if (strcmp(extention,"txt")==0) {
        return TXT_FILE;
    }
	else if (strcmp(suffix,"csv")==0) {
        return CSV_FILE;
    }
    else return UNKNOWN;
}

void handler(char * filename)(
    char * _pt_ = strchr(filename,'.');
    char * extention = malloc(strlen(_pt_++)+1);
    strcpy(extention,_pt_);
    
    int filetype = getType(extention);
    free(extention);
    switch (filetype) {
        case TXT_FILE: ...;
        case CSV_FILE: ...;
        case ... : ...;
        default: ...;
	}
    
	...;
    
}
```

