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
	