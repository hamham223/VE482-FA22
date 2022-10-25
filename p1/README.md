# ECE4820J Project 1

[![Build Status](https://focs.ji.sjtu.edu.cn:2222/api/badges/ECE482-22/YinchenNi520370910026-p1/status.svg)](https://focs.ji.sjtu.edu.cn:2222/ECE482-22/YinchenNi520370910026-p1)

## Info

> Author: Hamster.! :hamster:

This is a simple shell project developed by :hamster: in UM-SJTU JI. 

The shell is so called `mumsh`, it supports the following features:

+ single commands with/without args, e.g. `ls`, `ls -la`
+ Bash style file I/O redirection, e.g. `cat <1.txt > 2.txt`
+ Basic pipe, e.g. `echo 123 | grep 1`
+ Interruptions including `CTRL-D` and `CTRL-C`
+ Internal commands including `pwd` and `cd`
+ Handling errors for all supported features

## Build & Run

To build the project, simply type

```bash
make
```

To clean off `.o` and executable files, type

```bash
make clean
```
