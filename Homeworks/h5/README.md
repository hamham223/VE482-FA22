# ECE4820J Homework 5

> Author: :hamster:

### Ex 1

1. Deadlock cannot happen in this situation.  According to pigeonhole principle, there is one process using two resources and another one using only one. Since one process only need two resources and do not require any more, so after it finishes its job, the resources will be released and thus give to another resource. The condition of `hold and wait` is not satisfied. 

2. $n$ can be $1, 2, 3, 4, 5$. The reason is similar to the previous question. When $n=6$, a deadlock can form when they all hold one resource and wait for another resource.

3. We have the following: 

$$
\frac{35}{50}+\frac{20}{100}+\frac{10}{200}+\frac{x}{250}\leq 1
$$

  And we can solve that $x \leq 12.5$. Namely, the event can at most require for 12.5 ms of time.

4. It means the process will be run for a second time. The reason is to increase the priority of this process so that more CPU time can be given to this process.
5. + In source code: I/O bound process would involve lots of read/write codes while CPU bound process would involve lots of computational code.
   + At run time: Use `htop` and `iostat`.

### Ex 2

1. $$
   R := \begin{pmatrix} 7 & 4 & 3\\
                        1 & 2 & 2\\
                        6 & 0 & 0\\
                        0 & 1 & 1\\
                        4 & 3 & 1
                    \end{pmatrix}
   $$

2. The system is in a safe state. We can first let $P_2$ to run, then let $P_4$ to run. After they are finished, we have 743 available and thus $P_1$ can be run. Then we can finish $P_3$, and then $P_5$ respectively.

3. Yes. Order: $P_2 \to P_4 \to P_1 \to P_3 \to P_5$.

   Here's the snapshot table:

| Process | Allocated | Maximum | Request | Available |
| :-----: | :-------: | :-----: | :-----: | :-------: |
|         |           |         |         |    332    |
|  $P_2$  |    200    |   322   |   122   |    210    |
|         |           |         |         |    532    |
|  $P_4$  |    211    |   222   |   011   |    521    |
|         |           |         |         |    743    |
|  $P_1$  |    010    |   753   |   743   |    000    |
|         |           |         |         |    753    |
|  $P_3$  |    302    |   902   |   600   |    153    |
|         |           |         |         |  (10)55   |
|  $P_5$  |    002    |   433   |   431   |    624    |
|         |           |         |         |  (10)57   |

### Ex 3

1. ```C
   int count = 0; // the global reader counter
   
   void read_lock() {
       down(count_lock); // lock the reader counter
       if (++count == 1) down(db_lock); // only lock db for the first time
       up(count_lock); // unlock the reader counter
   }
   
   void read_unlock() {
       down(count_lock);
       if (--count == 0) up(db_lock);
       up(count_lock);
   }
   ```

2. They need to wait for the `count` to be released to them, as only one reader can change the `count` at one time. 

3. ```C
   void write_lock() {
       down(read_lock);
       down(db_lock);
   }
   
   void write_unlock() {
       up(db_lock);
       up(read_lock);
   }
   
   void new_read_lock() {
       down(read_lock);
       down(count_lock);
       if (++count == 1) down(db_lock);
       up(count_lock); 
       up(read_lock);
   }
   
   void new_read_unlock() {
       down(read_lock);
       down(count_lock);
       if (--count == 0) up(db_lock);
       up(count_lock);
       up(read_lock);
   }
   ```

4. The new solution gives priority to the writer over the reader. The old one gives priority to the reader.

   Therefore it is never solved...