#!/bin/bash
FILE=./number_norace
if ! [ -s $FILE ]; then
	# to judge whether it is empty
	echo 0 > $FILE
	# print the first number 0
fi
for i in $(seq 0 9); do
	(
		flock -e 200
    	n=$(tail -n 1 $FILE)
    	((n++))
    	echo $n >> $FILE
	) 200>> $FILE
done
	