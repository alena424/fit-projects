#!/bin/sh

# test projektu 3 IZP - 2016
# autor: ihynek

if [ "$1" = "" ] ; then
  path="./"
else
  path=$1
fi

proj="proj3"

echo "
============= BUILD TEST =============
"

echo "gcc -std=c99 -pedantic -Wall -Wextra -DNDEBUG -g -o "$path"$proj "$path"$proj.c -lm"
gcc -std=c99 -pedantic -Wall -Wextra -DNDEBUG -g -o "$path"$proj "$path"$proj.c -lm

echo "
============= VALGRIND TEST =============
"

echo "valgrind "$path"$proj input.txt"
valgrind "$path"$proj input.txt

echo "
============= OUTPUT FORMAT TEST =============
"

echo ""$path"$proj input.txt"
timeout 3s bash -c "\"$path\"$proj input.txt"
echo ""

echo ""$path"$proj input.txt -1"
timeout 3s bash -c "\"$path\"$proj input.txt -1"

echo "
============= CMP TEST =============
"

echo ""$path"$proj input.txt"
expected="Clusters:
cluster 0: 40[86,663] 43[747,938] 47[285,973] 49[548,422] 52[741,541] 56[44,854] 57[795,59] 61[267,375] 62[85,874] 66[125,211] 68[80,770] 72[277,272] 74[222,444] 75[28,603] 79[926,463] 83[603,68] 86[238,650] 87[149,304] 89[749,190] 93[944,835]"
expectedParsed=`echo "$expected" | sed "s/cluster [0-9]+: //g" | tail -n +2 | sort | sed ':a;N;$!ba;s/\n/ ~~~~ /g'`
result=`timeout 3s bash -c "\"$path\"$proj input.txt"`
resultParsed=`echo "$result" | sed "s/cluster [0-9]+: //g" | tail -n +2 | sort | sed ':a;N;$!ba;s/\n/ ~~~~ /g'`
if [ "$resultParsed" = "$expectedParsed" ] ; then
  echo "OK"
else 
  echo "$result"
  echo "ERR - EXPECTED:"
  echo "$expected"
fi
echo "---------------------
"

echo ""$path"$proj input.txt 8"
expected="Clusters:
cluster 0: 40[86,663] 75[28,603] 86[238,650]
cluster 1: 43[747,938] 93[944,835]
cluster 2: 47[285,973]
cluster 3: 49[548,422]
cluster 4: 52[741,541] 79[926,463]
cluster 5: 56[44,854] 62[85,874] 68[80,770]
cluster 6: 57[795,59] 83[603,68] 89[749,190]
cluster 7: 61[267,375] 66[125,211] 72[277,272] 74[222,444] 87[149,304]"
expectedParsed=`echo "$expected" | sed "s/cluster [0-9]+: //g" | tail -n +2 | sort | sed ':a;N;$!ba;s/\n/ ~~~~ /g'`
result=`timeout 3s bash -c "\"$path\"$proj input.txt 8"`
resultParsed=`echo "$result" | sed "s/cluster [0-9]+: //g" | tail -n +2 | sort | sed ':a;N;$!ba;s/\n/ ~~~~ /g'`
if [ "$resultParsed" = "$expectedParsed" ] ; then
  echo "OK"
else 
  echo "$result"
  echo "ERR - EXPECTED:"
  echo "$expected"
fi
echo "---------------------
"

echo ""$path"$proj input.txt 2016"
expected="Clusters:
cluster 0: 40[86,663]
cluster 1: 43[747,938]
cluster 2: 47[285,973]
cluster 3: 49[548,422]
cluster 4: 52[741,541]
cluster 5: 56[44,854]
cluster 6: 57[795,59]
cluster 7: 61[267,375]
cluster 8: 62[85,874]
cluster 9: 66[125,211]
cluster 10: 68[80,770]
cluster 11: 72[277,272]
cluster 12: 74[222,444]
cluster 13: 75[28,603]
cluster 14: 79[926,463]
cluster 15: 83[603,68]
cluster 16: 86[238,650]
cluster 17: 87[149,304]
cluster 18: 89[749,190]
cluster 19: 93[944,835]"
expectedParsed=`echo "$expected" | sed "s/cluster [0-9]+: //g" | tail -n +2 | sort | sed ':a;N;$!ba;s/\n/ ~~~~ /g'`
result=`timeout 3s bash -c "\"$path\"$proj input.txt 2016"`
resultParsed=`echo "$result" | sed "s/cluster [0-9]+: //g" | tail -n +2 | sort | sed ':a;N;$!ba;s/\n/ ~~~~ /g'`
if [ "$resultParsed" = "$expectedParsed" ] ; then
  echo "OK"
else 
  echo "$result"
  echo "ERR - EXPECTED:"
  echo "$expected"
fi
echo "---------------------
"
