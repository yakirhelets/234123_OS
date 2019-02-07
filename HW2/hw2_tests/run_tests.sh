#!/bin/bash

for i in `seq 1 9`;
  do
  	gcc -g -std=c99 test2`echo $i`.c -o test
		./test
	done
