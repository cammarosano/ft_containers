#!/bin/bash
if [ -z $1 ]
then
	echo "Please inform seed as parameter"
	exit 1
fi

make all
time ./test_stl $1
time ./test_ft $1
