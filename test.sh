#!/bin/bash

REPEAT=${2:-10}

for i in $(seq $REPEAT)
do
    ./build/sudoku input/$1/p0$i
done
