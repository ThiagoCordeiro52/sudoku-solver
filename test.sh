#!/bin/bash

ALL_FILES=(easy medium hard expert evil beginner16 confirmed16 expert16 beginner25 expert25)

if [[ $1 -eq all ]]
then
    for name in ${ALL_FILES[@]}
    do
        echo running $name
        for file in input/$name/*;
        do
            ./build/sudoku $file
        done
    done
else
    for file in /input/$1
    do
        ./build/sudoku input/$1/$file
    done
fi
