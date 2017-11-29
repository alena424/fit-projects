#!/bin/sh

# test projektu 2 IZP - 2016
# autor: ihynek

if [ "$1" = "" ] ; then
  path="./"
else
  path=$1
fi

proj="proj2"

echo "gcc -std=c99 -pedantic -Wall -Wextra -o "$path"$proj "$path"$proj.c -lm"
gcc -std=c99 -pedantic -Wall -Wextra -o "$path"$proj "$path"$proj.c -lm
echo "---------------------
"

echo ""$path"$proj --log 1.131401114526 4"
result=`timeout 3s bash -c "\"$path\"$proj --log 1.131401114526 4"`
echo " ----- EXPECTED:"
echo "       log(1.1314) = 0.123456789012"
echo " cfrac_log(1.1314) = 0.123456789012"
echo "taylor_log(1.1314) = 0.123452108537"
echo " ----- RESULT:"
echo "$result" | head -10 | cut -c 1-1000
echo "---------------------
"

echo ""$path"$proj --pow 1.23 4.2 4"
result=`timeout 3s bash -c "\"$path\"$proj --pow 1.23 4.2 4"`
echo " ----- EXPECTED:"
echo "         pow(1.23,4.2) = 2.38562110403"
echo "  taylor_pow(1.23,4.2) = 2.38026034593"
echo "taylorcf_pow(1.23,4.2) = 2.38079698151"
echo " ----- RESULT:"
echo "$result" | head -10 | cut -c 1-1000
echo "---------------------
"

echo ""$path"$proj --log 0 4"
result=`timeout 3s bash -c "\"$path\"$proj --log 0 4"`
echo " ----- EXPECTED:"
echo "       log(0) = -inf"
echo "    cf_log(0) = -inf"
echo "taylor_log(0) = -inf"
echo " ----- RESULT:"
echo "$result" | head -10 | cut -c 1-1000
echo "---------------------
"

echo ""$path"$proj --log -inf 4"
result=`timeout 3s bash -c "\"$path\"$proj --log -inf 4"`
echo " ----- EXPECTED:"
echo "       log(-inf) = nan"
echo "    cf_log(-inf) = nan"
echo "taylor_log(-inf) = nan"
echo " ----- RESULT:"
echo "$result" | head -10 | cut -c 1-1000
echo "---------------------
"

echo ""$path"$proj --pow 1.23 4.2 1000000"
result=`timeout 3s bash -c "\"$path\"$proj --pow 1.23 4.2 1000000"`
echo " ----- EXPECTED:"
echo "         pow(1.23,4.2) = 2.38562110403"
echo "  taylor_pow(1.23,4.2) = 2.38562110403"
echo "taylorcf_pow(1.23,4.2) = 2.38562110403"
echo " ----- RESULT:"
echo "$result" | head -10 | cut -c 1-1000
echo "---------------------
"
