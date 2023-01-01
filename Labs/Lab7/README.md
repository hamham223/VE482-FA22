# ECE4820J Lab 7

> Author: :car:

1) 	A kernel model can be loaded and unloaded to the kernel program at runtime, to extend the kernel function, and does not need to be re-compiled.
	When adding a regular library, the program need to be compiled again.
2) First write a makefile:

~~~makefile
obj-m = foo.o
KVERSION = $(shell uname -r)
all:
        make -C /lib/modules/$(KVERSION)/build M=$(PWD) modules
clean:
        make -C /lib/modules/$(KVERSION)/build M=$(PWD) clean
~~~
Then compile with `make` and load it with `insmod foo.ko`

3) 	There are 3 mutexes defined: `dadfs_sb_lock` locks super block when action is taken on block; `dadfs_inodes_mgmt_lock`  locks super block when block inode need to be changed. `dadfs_directory_children_update_lock` locks super block when children of the block is changed.
	The approach is good because it splits the functions of mutexes, so that it is made very clear.

4) Using `mmap()`, we can map the user processes to the memory allocated by the kernel. Then the process can use this address by pointers, and the system can directly write the files for sharing information.