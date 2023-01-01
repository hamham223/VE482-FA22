# ECE4820J Lab 4

> Author: :hamster:

### Database System

> https://www.statista.com/statistics/809750/worldwide-popularity-ranking-database-management-systems/#:~:text=As%20of%20August%202022%2C%20the%20most%20popular%20database,Microsoft%20SQL%20server%20rounded%20out%20the%20top%20three.

The most three popular database systems are: **Oracle**, **MySQL**, **Microsoft SQL Server**. 

#### Pros and Cons

> https://www.keycdn.com/blog/popular-databases

+ Oracle is robust but require lots of resources
+ MySQL is free but not handy
+ Microsoft SQL works well with Microsoft product and is fast, but very expensive

#### Database queries

1. ```sqlite
   select name, COUNT(hash) as counts 
   from time_stamp 
   group by name 
   order by counts desc 
   limit 5;
   ```
   result:
   ```
   Linus Torvalds|30702
   David S. Miller|13180
   Takashi Iwai|7726
   Mark Brown|7670
   Arnd Bergmann|7520
   ```
2. Unix Time:

   + `Jan 1st 2020`: 1577808000
   + `Jan 1st 2019`: 1546272000
   + `Jan 1st 2018`: 1514736000

   For example, for `2019` year only, to get the 5 top contributors

   ```sqlite
   select name, counts from (
       select name, tstamp, COUNT(hash) as counts 
       from time_stamp 
       where tstamp<1577808000 and tstamp >= 1546272000 
       group by name 
       order by counts desc
   ) 
   limit 5;
   ```

3. ```sqlite
   select comment, count(comment) as counts 
   from db 
   group by comment 
   order by counts desc 
   limit 5;
   ```
   
   result:
   
   ```
   Merge git://git.kernel.org/pub/scm/linux/kernel/git/davem/net|670
   Merge branch 'for-linus' of git://git.kernel.org/pub/scm/linux/kernel/git/dtor/input|301
   Merge branch 'x86-urgent-for-linus' of git://git.kernel.org/pub/scm/linux/kernel/git/tip/tip|275
   Merge git://git.kernel.org/pub/scm/linux/kernel/git/davem/net-2.6|262
   Merge branch 'perf-urgent-for-linus' of git://git.kernel.org/pub/scm/linux/kernel/git/tip/tip|248
   ```


4. ```sqlite
   select day, count(hash) as counts from (
       select hash, date(dates) as day 
       from time_stamp
   ) 
   group by day 
   order by counts desc 
   limit 5;
   ```

   result:

   ```
   2008-01-30|1031
   2006-12-07|683
   2007-05-08|649
   2013-07-03|626
   2007-10-16|613
   ```

5. Take the most contributor `Linux Torvalds` as an example:

   ```sqlite
   select (max(tstamp) - min(tstamp) ) / (count(name) - 1 ) 
   from time_stamp 
   where name = "Linus Torvalds";
   ```

   result: 

   ```
   15880
   ```

## Debugging

1. Add option `-g` when compiling

2. meaning of `gdb`: GNU debugger

3. homepage: https://www.sourceware.org/gdb/

4. Language support:
   +  Ada
   +  Assembly
   +  C
   +  C++
   +  D
   +  Fortran
   +  Go
   +  Objective-C
   +  OpenCL
   +  Modula-2
   +  Pascal
   +  Rust
   
   
