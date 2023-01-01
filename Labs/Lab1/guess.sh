#!/bin/bash
echo This is a simple guess number game!
echo Author: Hamtser
echo 
echo Instructions: The computer will randomly generate a number in 0-32767, each round you should enter a number and guess.
echo The computer will tell you whether the number should be larger or smaller.
echo If you get the correct number, you win.
echo
echo Game Start NOW!

a=$RANDOM
read input
while [ $input -ne $a ]; do
	if [ $input -gt $a ] ; then
		echo "Smaller than" $input
	else 
		echo "Larger than" $input
	fi
	read input
done
echo You Win!!!
