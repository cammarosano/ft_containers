#!/bin/bash
if [ -z $1 ]
then
	echo "Please inform seed as parameter"
	exit 1
fi

make bonus
time ./bonus_test_stl $1
time ./bonus_test_ft $1
