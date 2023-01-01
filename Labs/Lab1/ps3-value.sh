#!/bin/bash
echo "please select a value to display a month on the list below"
select i in jan feb mar apr exit
do
    case $i in
       jan) echo "January";;
       feb) echo "February";;
       mar) echo "March";;
       exit) exit;;
    esac
done
