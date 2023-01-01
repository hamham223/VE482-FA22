# ECE4820J Homework 1

> Author: :hamster:

### Ex 1

+ **Stack**:  storage for temporary variable in functions, used in recursion
+ **Heap**: storage for dynamic data, such as variables that are dynamically allocated

### Ex 2

1. > https://www.geeksforgeeks.org/what-happens-when-we-turn-on-computer/

   **Steps**:  

   + Power on Self Test
   + Master Boot Record
   + Init

   The BIOS chip contains bootstrap loaders (grub in Linux), which will start the real operating system. When the computer is turned on or restarted, it first performs the power-on-self-test. If the POST is successful and no issues are found, the bootstrap loader will load the operating system for the computer into memory

2. > https://www.tutorialspoint.com/what-is-kernel-and-different-types-of-kernels

   **Kernel**: an important part of an Operating System. The kernel is the first program that is loaded after the boot loader whenever we start a system. The Kernel is present in the memory until the OS is shut-down.

   Kernel provides an interface between the user and the hardware components of the system. When a process makes a request to the Kernel, then it is called System Call.

   + **Hybrid kernel**: combination of both Monolithic Kernel and Micro kernel. It uses the speed of the Monolithic Kernel and the modularity of Micro kernel.

     Hybrid kernels are micro kernels having some non-essential code in kernel-space in order for the code to run more quickly than it would be in user-space. So, some services like network stack or file systems are run in Kernel space to reduce the performance overhead, but still, it runs kernel code as servers in the user-space.

   + **Exo kernel**: developed by MIT and DOS. The resource protection is separated from the management which results in allowing us to perform application specific customization.

     In the Exo kernel, there is _no force abstraction_ and this is the feature that makes it different from a Monolithic Kernel and Micro kernel. But the drawback is the exo kernel has complex design.

### Ex 3

1. > https://www.geeksforgeeks.org/privileged-and-non-privileged-instructions-in-operating-system/#:~:text=The%20Instructions%20that%20can%20run%20only%20in%20Kernel,The%20Hardware%20traps%20it%20in%20the%20Operating%20System.

   The instructions that can run only in *Kernel Mode* are called **Privileged  Instructions**.

   Examples includes:

   + I/O instructions and Halt instructions
   + Turn off all Interrupts
   + Set the Timer
   + Context Switching
   + Clear the Memory or Remove a process from the Memory
   + Modify entries in the Device-status table

   So **a** and **c** should be executed only in kernel mode, **b** could be executed in user mode, while **d** should never be executed. (?)

2. It can be either 20, 25, or 30ms, depending upon the scheduler.

Total time is up to the longest time to finish all the tasks. The worst case is that one core execute for 5ms and the other gets 20ms and 10ms, and thus in 30ms in total.

### Ex 4

1. ```bash
   adduser
   ```

2. ```bash
   ps
   ```

3. ```bash
   top
   ```

4. ```bash
   head /dev/random | tee file1 file2
   ```

5. ```bash
   cat file2 >> file1
   ```

6. ```bash
   hexdump file1
   ```

7. ```bash
   find /usr/src -name "*semaphore*" | xargs grep -in "ddekit_sem_down"
   ```

See also `ex4.sh`.
