# ECE4820J Homework 8

> Author: :hamster:
>
> The LAST ONE!!!

### Ex 1

1. It is possible if the calculation-bound tasks and the I/O-bound tasks are totally separated. However in real life the scanning will cost calculation, meaning the sending task cannot be run with full speed.

2. Use those loadable modules that can be loaded and unloaded dynamically.

3. + a) and b): Device Drivers Layer
   + c) Device Independent OS Software Layer
   + d) User Level IO Software Layer

4. $$
   2 \times\frac{32 \text{~bits}}{100 \text{~ns}} = 6.4 \times 10^8 \text{~bits} \approx 80 \text{MB / s}
   $$

5. thin client: a virtual desktop computing model that runs on the resources stored on a central server instead of a computer's resources.

   > https://www.techtarget.com/searchnetworking/definition/thin-client

6. A hard disk is an electro-mechanical data storage device that stores and retrieves digital data using magnetic storage and one or more rigid rapidly rotating platters coated with magnetic material. The platters are paired with magnetic heads, usually arranged on a moving actuator arm, which read and write data to the platter surfaces.

### Ex 2

1. It could be used by pre-loading a 1 into the register, since 1 means lock. Then that register and the memory word are swapped atomically. 

2. In computing, **virtualization** or **virtualisation** (sometimes abbreviated **v12n**, a numeronym is the act of creating a virtual (rather than actual) version of something at the same abstraction level, including virtual computer hardware platforms, storage devices, and computer network resources.

   > https://en.wikipedia.org/wiki/Virtualization

   Challenges:
   
   + Compatibility
   + Storage and network
   + Security
   + Resource productivity problem
   
   
   > https://blogs.accops.com/common-virtualization-challenges/

### Ex 3

1. `/usr/lib/mutt`
2. 256^3+256^2+256 KB = 16 GB
3. 15000 rpm -> 0.25 r per ms
   + 1KB: $8 + \frac{1024\times 1}{262144} / 0.25 = 8.0156 [ms]$
   + 2KB: 8.03 ms
   + 4KB: 8.06 ms

### Ex 4

1. + Only lower case: $26^8 \approx 2 \times 10^{11}$
   + mix: more than $96^8 \approx 7.2 \times 10^{15}$ (96 out of 128 visible ascii char)
   + The latter one is at least 10000 times difficult to attack than the former one

2. + none-zero: usual small allcations
   + zero: page fault or not enough memory

3. > https://www.geeksforgeeks.org/system-security/

   According to the Wooden Barrel Theory, the security level of this system is determined by the weakest security level among the components. So, the system is probably not that secure.

4. Cannot achieve 100% safety. More bugs are written one a bug is fixed. :)

### Ex 5

> https://www.javatpoint.com/linux-firewall

A **firewall** can be defined as a system of network security that controls and filters the traffic on the rule's predefined set. It is an intermediary system between the Internet and the device.

#### iptables

Specifically, **iptables** recognizes the packets which are received and applies a group of rules for deciding what we can do to them.

`ipatables` is a **command-line interface** application that permits an  administrator for configuring particular rules that would enforce a  Linux kernel to implement an action like drop, modify, or inspect  network packets.


