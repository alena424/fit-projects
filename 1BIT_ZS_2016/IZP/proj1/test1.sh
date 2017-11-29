#!/bin/sh

# test projektu 1 IZP - 2016
# autor: ihynek

if [ "$1" = "" ] ; then
  path="./"
else
  path=$1
fi

proj="proj1"

echo "gcc -std=c99 -pedantic -Wall -Wextra -o "$path"$proj "$path"$proj.c"
gcc -std=c99 -pedantic -Wall -Wextra -o "$path"$proj "$path"$proj.c
echo "---------------------
"

echo "#1:"
echo "echo \"Ahoj\" | "$path"$proj"
result=`timeout 1s bash -c "echo \"Ahoj\" | \"$path\"$proj"`
if [ "$result" = "00000000  41 68 6f 6a 0a                                    |Ahoj.           |" ] ; then
  echo "OK"
else 
  echo " ----- EXPECTED:"
  echo "00000000  41 68 6f 6a 0a                                    |Ahoj.           |"
  echo " ----- RESULT:"
  if [ ${#result} -lt 100 ] ; then
    echo "$result"
  else
    echo "  ----- KILLED -----"
  fi
fi
echo "---------------------
"

echo "#2:"
echo "echo \"Hello, world! Ahoj svete!\" | "$path"$proj -s 14 -n 5"
result=`timeout 1s bash -c "echo \"Hello, world! Ahoj svete!\" | \"$path\"$proj -s 14 -n 5"`
if [ "$result" = "0000000e  41 68 6f 6a 20                                    |Ahoj            |" ] ; then
  echo "OK"
else 
  echo " ----- EXPECTED:"
  echo "0000000e  41 68 6f 6a 20                                    |Ahoj            |"
  echo " ----- RESULT:"
  if [ ${#result} -lt 100 ] ; then
    echo "$result"
  else
    echo "  ----- KILLED -----"
  fi
fi
echo "---------------------
"

echo "#3:"
echo "echo \"Hello\" | "$path"$proj -x"
result=`timeout 1s bash -c "echo \"Hello\" | \"$path\"$proj -x"`
if [ "$result" = "48656c6c6f0a" ] ; then
  echo "OK"
else 
  echo " ----- EXPECTED:"
  echo "48656c6c6f0a"
  echo " ----- RESULT:"
  if [ ${#result} -lt 100 ] ; then
    echo "$result"
  else
    echo "  ----- KILLED -----"
  fi
fi
echo "---------------------
"

echo "#4:"
echo "printf \"\\xff\\x00\" | "$path"$proj -x"
result=`timeout 1s bash -c "printf \"\xff\x00\" | \"$path\"$proj -x"`
if [ "$result" = "ff00" ] ; then
  echo "OK"
else 
  echo " ----- EXPECTED:"
  echo "ff00"
  echo " ----- RESULT:"
  if [ ${#result} -lt 100 ] ; then
    echo "$result"
  else 
    echo "  ----- KILLED -----"
  fi
fi
echo "---------------------
"

echo "#5:"
echo "printf \"Ahoj\\0Slovo\\nXY\\n\" | "$path"$proj -S 3"
result=`timeout 1s bash -c "printf \"Ahoj\0Slovo\nXY\n\" | \"$path\"$proj -S 3"`
if [ "$result" = $'Ahoj\nSlovo' ] ; then
  echo "OK"
else 
  echo " ----- EXPECTED:"
  echo -e "Ahoj\nSlovo"
  echo " ----- RESULT:"
  if [ ${#result} -lt 100 ] ; then
    echo "$result"
  else
    echo "  ----- KILLED -----"
  fi
fi
echo "---------------------
"

echo "#6:"
echo "echo \"48656c6c0a\" | "$path"$proj -r"
result=`timeout 1s bash -c "echo \"48656c6c0a\" | \"$path\"$proj -r"`
if [ "$result" = "Hell" ] ; then
  echo "OK"
else 
  echo " ----- EXPECTED:"
  echo "Hell"
  echo " ----- RESULT:"
  if [ ${#result} -lt 100 ] ; then
    echo "$result"
  else
    echo "  ----- KILLED -----"
  fi
fi
echo "---------------------
"

echo "#7:"
echo "echo \"00ff00ff\" | "$path"$proj -r | "$path"$proj -x"
result=`timeout 1s bash -c "echo \"00ff00ff\" | \"$path\"$proj -r | \"$path\"$proj -x"`
if [ "$result" = "00ff00ff" ] ; then
  echo "OK"
else 
  echo " ----- EXPECTED:"
  echo "00ff00ff"
  echo " ----- RESULT:"
  if [ ${#result} -lt 100 ] ; then
    echo "$result"
  else
    echo "  ----- KILLED -----"
  fi

fi
echo "---------------------
"
