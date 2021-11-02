#!/bin/bash
if [ -z $1 ]
then
	echo "Please inform seed as parameter"
	exit 1
fi

make all

printf "\033[32;1mTesting STL containers: \033[0m\n"
time ./test_stl $1

printf "\n\033[36;1mTesting FT containers: \033[0m\n"
time ./test_ft $1
