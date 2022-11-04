#!/bin/bash

if [ $# -ne 2 ]
then
    echo "Usage $0: director_sursa director_destinatie"
    exit 1  
fi

flag=0
if ! [ -d $1 ]
then
    echo "Primul argument nu este director."
    flag=1
fi

if ! [ -d $2 ]
then
    echo "Al doilea argument nu este director."
    flag=1
fi

if [ $flag -eq 1 ]
then
    exit 2
fi

statistics="$2/statistica.txt"
touch statistics
echo -n "" > $statistics

count_files=0
count_lines=0
regex="^[A-Z][a-z]* - [A-Z][a-z]* [0-9]{2}.[0-9]{2}.[0-9]{4}$"
for entry in $1/*
do
    if [ -f $entry ] && [ ${entry: -4} == ".txt" ]
    then
        ((++count_files))
        count_valid=0
        while read line
        do
            ((++count_lines))
            if [[ $line =~ $regex ]]
            then
                ((++count_valid))
            fi
        done < $entry
        relative_name=${entry:$((${#1}+1)):${#entry}}
        echo "$relative_name: $count_valid" >> $statistics
    fi
done
echo "Au fost prelucrate $count_files fisiere, avand intotal $count_lines linii (valide)." >> $statistics