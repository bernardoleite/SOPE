#!/bin/bash

x=1

while [ $x -le 14 ]
do
mkdir prob$x
x=$(( $x + 1 ))
done
