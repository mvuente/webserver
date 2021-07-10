#!/bin/bash

i=$1
while [ $i -gt 0 ]  
do
	bash ./extra.sh $2 && echo done!!! &
	i=$((i - 1))
	echo $i
done


