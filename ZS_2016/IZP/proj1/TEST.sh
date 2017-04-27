#!/bin/bash  
# Testování projektu 1 IZP
# xratko00

# Odstraneni starych souboru
rm ./ProjResult/Basis/*.out ./ProjResult/Basis/*.err &> /dev/null
rm ./ProjResult/Extend/*.out ./ProjResult/Extend/*.err &> /dev/null

echo "-----------------------------"
echo "*  ZAKLADNI TESTY IZP WIKI  *"
echo "-----------------------------"

# TEST1
echo "Hello, world! Ahoj svete!" | ../proj1 > ./ProjResult/Basis/01.out
 
diff ./ProjResult/Basis/01.out ./RefResult/Basis/01.ref > ./ProjResult/Basis/01.err
if [ $? == 0 ]; then
    echo "->Test 1  [OK]"
else
    echo "->Test 1  [BAD]"
fi    
 
# TEST2
echo "Hello, world! Ahoj svete!" | ../proj1 -s 14 -n 5 > ./ProjResult/Basis/02.out

diff ./ProjResult/Basis/02.out ./RefResult/Basis/02.ref > ./ProjResult/Basis/02.err
if [ $? == 0 ]; then
    echo "->Test 2  [OK]"
else
    echo "->Test 2  [BAD]"
fi

# TEST3 
echo "Hello" | ../proj1 -x > ./ProjResult/Basis/03.out

diff ./ProjResult/Basis/03.out ./RefResult/Basis/03.ref > ./ProjResult/Basis/03.err
if [ $? == 0 ]; then
    echo "->Test 3  [OK]"
else
    echo "->Test 3  [BAD]"
fi

# TEST4 
printf 'Hello, world!\0Ahoj svete!\n\0AP\nABCD\n' | ../proj1 -S 3 > ./ProjResult/Basis/04.out

diff ./ProjResult/Basis/04.out ./RefResult/Basis/04.ref > ./ProjResult/Basis/04.err
if [ $? == 0 ]; then
    echo "->Test 4  [OK]"
else
    echo "->Test 4  [BAD]"
fi

# TEST5 
echo "48 65 6c6c6f a" | ../proj1 -r > ./ProjResult/Basis/05.out

diff ./ProjResult/Basis/05.out ./RefResult/Basis/05.ref > ./ProjResult/Basis/05.err
if [ $? == 0 ]; then
    echo "->Test 5  [OK]"
else
    echo "->Test 5  [BAD]"
fi

echo "-----------------------------"
echo "*   ROZSIRENE TESTY ARG OK  *"
echo "-----------------------------"

# ==============TESTY PROGRAMU BEZ ARGUMENTU==============
# EXTEND TEST1
echo "Testovaniprojek" | ../proj1 > ./ProjResult/Extend/01.out
 
diff ./ProjResult/Extend/01.out ./RefResult/Extend/01.ref > ./ProjResult/Extend/01.err
if [ $? == 0 ]; then
    echo "->Extend test 1  [OK]"
else
    echo "->Extend Test 1  [BAD]"
fi 

# EXTEND TEST2
printf 'Hello, world!\0Ahoj svete!\n\0AP\nABCD\n' | ../proj1 > ./ProjResult/Extend/02.out
 
diff ./ProjResult/Extend/02.out ./RefResult/Extend/02.ref > ./ProjResult/Extend/02.err
if [ $? == 0 ]; then
    echo "->Extend test 2  [OK]"
else
    echo "->Extend Test 2  [BAD]"
fi 

# EXTEND TEST3
printf '\a\b\c\b' | ../proj1 > ./ProjResult/Extend/03.out
 
diff ./ProjResult/Extend/03.out ./RefResult/Extend/03.ref > ./ProjResult/Extend/03.err
if [ $? == 0 ]; then
    echo "->Extend test 3  [OK]"
else
    echo "->Extend Test 3  [BAD]"
fi
# ==============TESTY PROGRAMU S ARGUMENTEM (-s)==============
# EXTEND TEST4
echo "Test češtiny ěščřžýáíé" | ../proj1 -s 0 > ./ProjResult/Extend/04.out
 
diff ./ProjResult/Extend/04.out ./RefResult/Extend/04.ref > ./ProjResult/Extend/04.err
if [ $? == 0 ]; then
    echo "->Extend test 4  [OK]"
else
    echo "->Extend Test 4  [BAD]"
fi
# EXTEND TEST5
echo "Přeskočení celého vstupu" | ../proj1 -s 200 > ./ProjResult/Extend/05.out
 
diff ./ProjResult/Extend/05.out ./RefResult/Extend/05.ref > ./ProjResult/Extend/05.err
if [ $? == 0 ]; then
    echo "->Extend test 5  [OK]"
else
    echo "->Extend Test 5  [BAD]"
fi
# EXTEND TEST6
echo "Test vypisu pouze do poloviny znaku" | ../proj1 -s 28 > ./ProjResult/Extend/06.out
 
diff ./ProjResult/Extend/06.out ./RefResult/Extend/06.ref > ./ProjResult/Extend/06.err
if [ $? == 0 ]; then
    echo "->Extend test 6  [OK]"
else
    echo "->Extend Test 6  [BAD]"
fi
# ==============TESTY PROGRAMU S ARGUMENTEM (-n)==============
# EXTEND TEST7
echo "Test -n" | ../proj1 -n 1 > ./ProjResult/Extend/07.out
 
diff ./ProjResult/Extend/07.out ./RefResult/Extend/07.ref > ./ProjResult/Extend/07.err
if [ $? == 0 ]; then
    echo "->Extend test 7  [OK]"
else
    echo "->Extend Test 7  [BAD]"
fi
# EXTEND TEST8
echo "Test -n" | ../proj1 -n 10 > ./ProjResult/Extend/08.out
 
diff ./ProjResult/Extend/08.out ./RefResult/Extend/08.ref > ./ProjResult/Extend/08.err
if [ $? == 0 ]; then
    echo "->Extend test 8  [OK]"
else
    echo "->Extend Test 8  [BAD]"
fi
# EXTEND TEST9
printf 'Test -n ABCDEFGKIJKLMN\0\n\n\n\n\' | ../proj1 -n 24 > ./ProjResult/Extend/09.out
 
diff ./ProjResult/Extend/09.out ./RefResult/Extend/09.ref > ./ProjResult/Extend/09.err
if [ $? == 0 ]; then
    echo "->Extend test 9  [OK]"
else
    echo "->Extend Test 9  [BAD]"
fi
# ==============TESTY PROGRAMU S ARGUMENTEM (-S)==============
# EXTEND TEST10
cat ./RefResult/Extend/10.ref | ../proj1 -S 111 > ./ProjResult/Extend/10.out
 
diff ./ProjResult/Extend/10.out ./RefResult/Extend/10.ref > ./ProjResult/Extend/10.err
if [ $? == 0 ]; then
    echo "->Extend test 10 [OK]"
else
    echo "->Extend Test 10 [BAD]"
fi 
# EXTEND TEST11
cat ./RefResult/Extend/11.ref | ../proj1 -S 10 > ./ProjResult/Extend/11.out
 
diff ./ProjResult/Extend/11.out ./RefResult/Extend/11.ref > ./ProjResult/Extend/11.err
if [ $? == 0 ]; then
    echo "->Extend test 11 [OK]"
else
    echo "->Extend Test 11 [BAD]"
fi 
# EXTEND TEST12
printf '\0\nHello,\tworld!\0Ahoj svete!\n\n\0CCD\0A\nABCD\n' | ../proj1 -S 2 > ./ProjResult/Extend/12.out
 
diff ./ProjResult/Extend/12.out ./RefResult/Extend/12.ref > ./ProjResult/Extend/12.err
if [ $? == 0 ]; then
    echo "->Extend test 12 [OK]"
else
    echo "->Extend Test 12 [BAD]"
fi 
# ==============TESTY PROGRAMU S ARGUMENTEM (-r)==============
# EXTEND TEST13
echo "00ff00ff" | ../proj1 -r > ./ProjResult/Extend/13.out
 
diff ./ProjResult/Extend/13.out ./RefResult/Extend/13.ref > ./ProjResult/Extend/13.err
if [ $? == 0 ]; then
    echo "->Extend test 13 [OK]"
else
    echo "->Extend Test 13 [BAD]"
fi 
# EXTEND TEST14
echo "0a 0a 0a 0a 38 0b 41 0b 61 0a 0a 0a 09 38 09 41 09 61 0a 0a 0a" | ../proj1 -r > ./ProjResult/Extend/14.out
 
diff ./ProjResult/Extend/14.out ./RefResult/Extend/14.ref > ./ProjResult/Extend/14.err
if [ $? == 0 ]; then
    echo "->Extend test 14 [OK]"
else
    echo "->Extend Test 14 [BAD]"
fi 
# EXTEND TEST15
printf "Ahoj jak se máš české znaky atd.\n\n\n" | ../proj1 -x  | ../proj1 -r > ./ProjResult/Extend/15.out
 
diff ./ProjResult/Extend/15.out ./RefResult/Extend/15.ref > ./ProjResult/Extend/15.err
if [ $? == 0 ]; then
    echo "->Extend test 15 [OK]"
else
    echo "->Extend Test 15 [BAD]"
fi 
# ==============TESTY PROGRAMU S ARGUMENTEM (-x)==============
# EXTEND TEST16
printf 'Ahoj jak se máš české znaky atd.\n\n\n' | ../proj1 -x > ./ProjResult/Extend/16.out
 
diff ./ProjResult/Extend/16.out ./RefResult/Extend/16.ref > ./ProjResult/Extend/16.err
if [ $? == 0 ]; then
    echo "->Extend test 16 [OK]"
else
    echo "->Extend Test 16 [BAD]"
fi 
# EXTEND TEST17
printf '' | ../proj1 -x > ./ProjResult/Extend/17.out
 
diff ./ProjResult/Extend/17.out ./RefResult/Extend/17.ref > ./ProjResult/Extend/17.err
if [ $? == 0 ]; then
    echo "->Extend test 17 [OK]"
else
    echo "->Extend Test 17 [BAD]"
fi
# EXTEND TEST18
printf '\0\1\2\3\4\5\6\7\8\9Ahoj jak se máš české znaky atd.\n\n\n' | ../proj1 -x > ./ProjResult/Extend/18.out
 
diff ./ProjResult/Extend/18.out ./RefResult/Extend/18.ref > ./ProjResult/Extend/18.err
if [ $? == 0 ]; then
    echo "->Extend test 18 [OK]"
else
    echo "->Extend Test 18 [BAD]"
fi
# EXTEND TEST19
echo '\0\1\2\3\4\5\6\7\8\9Ahoj jak se máš české znaky atd.\n\n\n' | ../proj1 -x > ./ProjResult/Extend/19.out
 
diff ./ProjResult/Extend/19.out ./RefResult/Extend/19.ref > ./ProjResult/Extend/19.err
if [ $? == 0 ]; then
    echo "->Extend test 19 [OK]"
else
    echo "->Extend Test 19 [BAD]"
fi
# ==============TESTY PROGRAMU S ARGUMENTEM (-n , -s)==============
# EXTEND TEST20
printf '' | ../proj1 -n 10 -s 20 > ./ProjResult/Extend/20.out
 
diff ./ProjResult/Extend/20.out ./RefResult/Extend/20.ref > ./ProjResult/Extend/20.err
if [ $? == 0 ]; then
    echo "->Extend test 20 [OK]"
else
    echo "->Extend Test 20 [BAD]"
fi
# EXTEND TEST21
printf '\0\nBBB \n\n\0BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\0' | ../proj1 -n 20 -s 5 > ./ProjResult/Extend/21.out
 
diff ./ProjResult/Extend/21.out ./RefResult/Extend/21.ref > ./ProjResult/Extend/21.err
if [ $? == 0 ]; then
    echo "->Extend test 21 [OK]"
else
    echo "->Extend Test 21 [BAD]"
fi
# EXTEND TEST22
printf '\0\nBBB \n\n\0BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\0' | ../proj1 -n 20 -s 200 > ./ProjResult/Extend/22.out
 
diff ./ProjResult/Extend/22.out ./RefResult/Extend/22.ref > ./ProjResult/Extend/22.err
if [ $? == 0 ]; then
    echo "->Extend test 22 [OK]"
else
  echo "->Extend Test 22 [BAD]"
fi
# ==============TESTY PROGRAMU S ARGUMENTEM  pridano==============
# EXTEND TEST23
printf 'křečovitě' | ../proj1 -S 1 > ./ProjResult/Extend/23.out
 
diff ./ProjResult/Extend/23.out ./RefResult/Extend/23.ref > ./ProjResult/Extend/23.err
if [ $? == 0 ]; then
    echo "->Extend test 23 [OK]"
else
  echo "->Extend Test 23 [BAD]"
fi
# ==============TESTY VELKYCH DAT==============
echo "-----------------------------"
echo "* ROZSIRENE TESTY BIG FILES *"
echo "-----------------------------"
# EXTEND TEST24
cat ./Texts/README | ../proj1 > ./ProjResult/Extend/24.out
 
diff ./ProjResult/Extend/24.out ./RefResult/Extend/24.ref > ./ProjResult/Extend/24.err
if [ $? == 0 ]; then
    echo "->Extend test 24 [OK]"
else
  echo "->Extend Test 24 [BAD]"
fi
# EXTEND TEST25
cat ./Texts/workqueue.c | ../proj1 -x > ./ProjResult/Extend/25.out
 
diff ./ProjResult/Extend/25.out ./RefResult/Extend/25.ref > ./ProjResult/Extend/25.err
if [ $? == 0 ]; then
    echo "->Extend test 25 [OK]"
else
  echo "->Extend Test 25 [BAD]"
fi
# EXTEND TEST26
cat ./Texts/workqueue.c | ../proj1 -x | ../proj1 -r > ./ProjResult/Extend/26.out
 
diff ./ProjResult/Extend/26.out ./RefResult/Extend/26.ref > ./ProjResult/Extend/26.err
if [ $? == 0 ]; then
    echo "->Extend test 26 [OK]"
else
  echo "->Extend Test 26 [BAD]"
fi
# EXTEND TEST27
cat ./Texts/workqueue.c | ../proj1 -S 100 > ./ProjResult/Extend/27.out
 
diff ./ProjResult/Extend/27.out ./RefResult/Extend/27.ref > ./ProjResult/Extend/27.err
if [ $? == 0 ]; then
    echo "->Extend test 27 [OK]"
else
  echo "->Extend Test 27 [BAD]"
fi
# EXTEND TEST28
cat ./Texts/workqueue.c | ../proj1 -S 199 > ./ProjResult/Extend/28.out
 
diff ./ProjResult/Extend/28.out ./RefResult/Extend/28.ref > ./ProjResult/Extend/28.err
if [ $? == 0 ]; then
    echo "->Extend test 28 [OK]"
else
  echo "->Extend Test 28 [BAD]"
fi
# EXTEND TEST29
cat ./Texts/workqueue.c | ../proj1 -s 65536 n 262144  > ./ProjResult/Extend/29.out
 
diff ./ProjResult/Extend/29.out ./RefResult/Extend/29.ref > ./ProjResult/Extend/29.err
if [ $? == 0 ]; then
    echo "->Extend test 29 [OK]"
else
  echo "->Extend Test 29 [BAD]"
fi
echo "-----------------------------"



#cisteni
find ./ProjResult/ -name *.err -size 0 -delete



