# VE482 Lab4

## Database

As the focus of our course is not database itself, we mainly focus on what you will spend lots of time on in p2. SQL.

### Installation of `sqlite3`

```shell
sudo apt install sqlite3
sqlite3
```

### Download Dataset

Canvas - Files - labs - lab4 dataset

`db.psv`, and `timestamp.psv`, here, `psv` means pipe separated values.

### Import Data

To create a database, we write

```shell
sqlite3 l4.db
```

And we can create tables inside database.

```sqlite
CREATE TABLE db
(
	hash TEXT NOT NULL,
    name TEXT NOT NULL,
    comment TEXT NOT NULL  
);
CREATE TABLE time_stamp
(
	hash TEXT NOT NULL,
    name TEXT NOT NULL,
    dates TEXT,
    tstamp INT
);
```

In SQLite you can either write commands in capital letters or smaller letters. Here I write capital for clarity. The `NOT NULL` keyword means that we don't allow `null` values in the column, or we delete those rows with empty entries.

Now we can import the csv files to the tables

```sqlite
.separator "|"
.import db.psv db
.import timestamp.psv time_stamp
```

You may find some error message occurring due to extra columns. This is because there can be extra pipes in the comment. You don't need to worry about it since we are not using the column `comment`.

## Basic Data Selection using SQLite3

For basic sql language grammars, I refer to 

[Aggregate and Aggregate Statistical - Apache Drill](https://drill.apache.org/docs/aggregate-and-aggregate-statistical/)

Apache Drill which is another powerful sql handle application developed in Java.

```sql
#select all the columns from db table and limit the number of row to 5 ----> useful to have a check of how the table looks like after each filtering
SELECT * FROM db LIMIT 5; 
#select certain column/columns
SELECT name FROM db LIMIT 5; 
SELECT hash,comment FROM db LIMIT 5;
#count is used to count the number of certain records 
SELECT COUNT(name) FROM db;
SELECT COUNT(name) FROM db WHERE name == 'Linus Torvalds';
#you can also give the new column a name --> this is extremely useful when you have multiple operations
SELECT COUNT(name) as counts FROM db WHERE name == 'Linus Torvalds';

#Grouping columns must be declared when the query computes aggregates with standard functions such as SUM, AVG, and COUNT 
SELECT name, COUNT(name) as counts FROM db group by name limit 5;
#Let's see how the size of table changes (the result of select is also a table)
SELECT count(*) as count FROM db;
SELECT count(*) as count FROM (SELECT name, COUNT(name) as counts FROM db group by name);
#what does the size of the previous table means?
#Then we can use ORDER as what we do in excel
SELECT name, COUNT(name) as counts FROM db group by name ORDER by counts limit 5;
SELECT name, COUNT(name) as counts FROM db group by name ORDER by counts desc limit 5;
#Some other commands you may need
#To fetch the date out
select date(dates) as date from time_stamp limit 5;

#Let's have a short demo! (aiming to help you solve the last sql in lab)
#Find how many time Linus have contributed to the kernel and the earilest and the latest time he has contributed to the kernel
#First, let's have a look at the time_stample table which we should use.
select * from time_stamp limit 5;
#what we need is `name` and `tstamp` record, we take them out.
select name, tstamp from time_stamp limit 5;
#Also, as we need to count the contribution times, we need count.
select name, count(name) as count, tstamp from time_stamp limit 5;#what's wrong with this?
select name, count(name) as count, tstamp from time_stamp group by name limit 5;#what's wrong with this?
select name, count(name) as count, max(tstamp) as latest_time, min(tstamp) as earliest_time from time_stamp group by name limit 5;
#Then five main contributors and average are left for you as we have introduced all the necessary parts before :(
#Then let's quit the sqlite3 or you will be stuck there.
.quit or Ctrl+D
```

