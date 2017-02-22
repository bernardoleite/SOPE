#!/bin/bash

file=$1
file_c=$1.c
file_o=$1.o

rm -f $file_o

cc -o $file $file_c

rsp="$(echo $?)"

if ($rsp != $0) ; then
	echo COMPILATION ERROR 
fi

./$file
~