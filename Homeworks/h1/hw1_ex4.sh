adduser 
ps
top
head /dev/random | tee file1 file2
cat file2 >> file1
hexdump file1
find /usr/src -name "*semaphore*" | xargs grep -in "ddekit_sem_down"