#!/bin/bash

rm stats.csv

echo "Starting"

tanks=23
for i in {23..60}
do
	echo $i
	#echo $tanks
	printf "$i," >> stats.csv
	./brewery $i >> stats.csv
	echo "" >> stats.csv
	#tanks+=1
done

echo "Everything went well"
