# LemonDB Recovered Files

## Introduction
A multi-thread key-value database for Lemonion  Inc.
Based on the original database, it supports 17 basic commands of the database. The running speed is improved through the implementation of multithreading.
This is Project 2 of ECE4820J. We implemented and released both single-threaded and multithreaded versions. We use the basic structure of the original Lemonion Inc. and mainly implement the functions and classes that deal with all kinds of queries in `/src/query/management` and `/src/query/data`.
## Install
### Prerequisites
1. C++ compiler: Clang >= 6.0
2. CMake >= 2.7
### Quick Start
First download the source code. Then open `/src` directory. Run the following commands.
```
$ mkdir build 
$ cd build
$ cmake .. && make -j8
$ cd ..
```
After compiling successfully, you will find that `lemondb` file is generated in `/build`. Then run the following command to start the database.
```
$ ./build/lemondb
```
## Documentation
### Design 
- We implement multi-thread in each single query. For queries that used for basic arithmetics such as `MIN`, `MAX` in a large table, we devided it into several parts. Each thread is responsible for processing a part of data. After the simultaneous calculation of these part, we combine the partial result into the final answer.We also apply multi-thread in queries of data manipulation such as `UPDATE`, `SELECT`.
- For commands that do not require traversing the entire table, such as `INSERT`, and table management queries like `LOAD`, `COPYTABLE`, we did not add multi-thread.
- Because we didn't the multi-thread bwtween the queries, queries will still be executed in order.

### Performance Improvements
- To make sure that the records can be displayed
in ascending lexical order, sorted by `KEY` and decrease the time used, we use `priority_queue` to store needed data row in `SELECT`. Because the inserted data is out of order, the time complexity is too high to maintain an ordered structure such as a red-black tree for each insertion. So we only sorted in `SELECT`. `qsort` or `merge sort` still results in high time and space complexity. Rather than sort the total table, we only push the selected data into the priority queue.
- To avoid the overuse of multi-theard, we limit the number of theard in some queries. We difine a `suggested_threads` as an optimal number. When the number of threads in argument is larger than this value, we will use the optimal value.

### Problems Solved
When implementing multithreading, we don't encounter many difficulties like deadlocks or mutex. But there are still some details that need attention.
- Before running multithreading, we need to check the number of threads the hardware can support. We used the function `hardware_concurrency()` to examinate it.
- Because of the frequent use of dynamically requested storage space, we must be careful with memory leaks.
- It is also worth noting that parameters should be passed using pointers rather than references.This is a problem detected in pratical operation, but the cause is unknown.

### Further Improvements
The database can be further improved in the following ways.
- Multi-thread between queries can be implemented to further  improve efficiency systematically and significantly.
- More elegant implementations of `SELECT` are believed to exist which can display data in ascending lexical order of `KEY`. How to deal with the balance between the ordered structure and the complexity of time and space is still worth considering.
- Vector is used to store data which benifits the efficient random access of data. However, `INSERT` and `DELETE` become less efficient because making a copy of the memory block with the time complexity `o(n)`. `DELETE` query may be futher improved to avoid too much single delete instructions in vector.

## Copyright
Lemonion Inc. 2018

