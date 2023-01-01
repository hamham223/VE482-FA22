#!/usr/bin/zsh

# must first su!!!

make clean
read -s $input
make
rmmod -f dicedevice
read -s $input
insmod dicedevice.ko
ls /dev/ -l | grep dice
read -s $input
cat /dev/hamster_dice_1
echo "DEBUG: The return value is: $?"
read -s $input
rmmod -f dicedevice
