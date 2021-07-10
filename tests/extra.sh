#!/bin/bash

i=$1

while [ $i -gt 0 ]
do
	curl -s http://localhost:1028 > /dev/null
	i=$[ $i - 1 ]
done


