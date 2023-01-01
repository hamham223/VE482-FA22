# ECE4820J Lab 5

> Author: :hamster:

### Ex 1: Layer Programming

The program has three different layers:

+ Data structure layer: Linked list
+ Logistic layer: Sorting algorithm
+ Interface layer: User interface (I/O)

They are split in five files:

+ `list.c`  & `list.h`
+ `sort.c` & `sort.h`
+ `ex2.c`

### Ex 2: Libraries

#### Compiling Procedure

> https://www.scaler.com/topics/c/compilation-process-in-c/

+ Preprocessing: Remove comments, expand macros, include header files
+ Compiling: Convert the immediate file(`.i`) into assembly file (`.s`)
+ Assembling: Use assembler to convert `.s` into object files (`.o`)
+ Linking: Include libraries and link `.o` files into executable files

#### Static Libs && Dynamic Libs

##### Difference

+ static: Reusable in multiple programs, are locked into a program at com-pile time. Usually very big and named `libxxx.a`.
+ dynamic: Lives outside of the executable file, can be modified without re-compile. Must provide the corresponding libraries when the programming is running. Usually named `libxxx.so` or `.dll` .

##### Create static libs

```bash
gcc -c list.c -o list.o
gcc -c sort.c -o sort.o
ar -rcs list.a list.o
ar -rcs sort.a sort.o
```

##### Compile with static libs

```bash
gcc -c ex2.c -o ex2.o
gcc -o main ex2.o -L. -llist -lsort
```

##### Compile with dynamic libs

> https://www.cs-fundamentals.com/c-programming/static-and-dynamic-linking-in-c

```bash
gcc *.c -c -fpic
gcc -shared -o liblist.so list.o
gcc -shared -o libsort.so sort.o
gcc -o main ex2.c -L. -llist -lsort
```

#### Difference: Libs V.S. API

> https://rapidapi.com/blog/api-vs-library/#:~:text=The%20main%20difference%20is%20that%20the%20library%20refers,of%20several%20libraries%20to%20complete%20a%20particular%20action.

| Library                      | API                                                          |
| :--------------------------- | :----------------------------------------------------------- |
| A reusable chunk of codes    | A point of contact that allows interaction between running codes |
| It refers to the code itself | It refers to the interface                                   |
| It is not an API by itself   | It can be made of several libraries                          |

### Ex 3

See `lab5_dlist.c`.
