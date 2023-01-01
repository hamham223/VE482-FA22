# ECE4820J Homework 3

> Author: :hamster:

### Ex 1

1. No. For a single-threaded process, when it is waiting for user input, it's in `blocked` states, it cannot `fork`.
2. If calls do not have win32 api equivalents, they might not run properly when being ported from UNIX to Windows.

### Ex 2



### Ex 3

> https://linuxhint.com/posix-standard/

+ **POSIX** stands for Portable Operating System Interface.

+ What is it?

  ```
  It is a IEEE standard that helps compatibility and portability between operating systems.
  ```

+ Why it exist?

  ```
  The introduction of new operating systems make it more complex to port applications across hardware, operating systems, and vendors. So POSIX standard was developed in the 1980s to resolve the portability issue.
  ```

+ What are included?

  + **Base Definition Volume:** General terms, concepts, and interfaces.
  + **Systems Interfaces Volume:** Definitions of system service functions and subroutines. Also, includes portability, error handling and error recovery.
  + **Shell and Utilities Volume:** Definition of interfaces of any application to command shells and common utility programs.
  + **Rationale Volume:** Contains information and history about added or discarded features and the reasonings of the decisions.