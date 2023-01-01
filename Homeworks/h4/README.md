# ECE4820J Homework 4

> Author: :hamster:

### Ex 1

1. When the OS is about to unblocking or blocking a thread, changing the thread scheduling.

   Possible solution: When the scheduler wants to interrupt the thread, set a flag (could be a global variable send to the thread). When the thread ends its program and see the flag, knowing an interrupt has happened, then it stops.

2. It's possible. We can implement some threads in user space to be interrupts (scheduler's effort) for other threads to do I/O.

### Ex 2

#### bash script:

```bash
#!/bin/zsh
FILE=./number
if ! [ -s $FILE ]; then
	# to judge whether it is empty
	echo 0 > $FILE
	# print the first number 0
fi
for i in $(seq 0 9); do
	# append 10 numbers every round
    n=$(tail -n 1 $FILE)
    ((n++))
    echo $n >> $FILE
done
```

#### run

Run the script both in foreground and in background

```bash
bash ./ex2.sh & bash ./ex2.sh
```

#### output

```
0 1 1 2 2 3 3 4 4 5 5
6 6 7 7 8 8 9 9 10 10
```

They are printing the same thing, indicating a race condition.

#### solution

This bash script uses `flock` to avoid the race condition.

```bash
#!/bin/bash
FILE=./number_norace
if ! [ -s $FILE ]; then
	echo 0 > $FILE
fi
for i in $(seq 0 9); do
	(
		flock -e 200
    	n=$(tail -n 1 $FILE)
    	((n++))
    	echo $n >> $FILE
	) 200>> $FILE
done
```

### Ex 3

See `cthread.c`.

Main usage:

+ `sem_wait(&sem)`
+ `sem_post(&sem)`
+ `sem_init(&sem, 0, 1)`
+ `sem_destroy(&sem)`
