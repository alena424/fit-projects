#!/bin/bash

RED='\033[1;31m'
GREEN='\033[1;32m'
NC='\033[0m'
FAULTS=0

if [ $# -eq 1 ] && [ $1 = "--show" ];
	then
		printf "Test:\t\t\t\t\tCorrect value:\n1.  log (-inf)\t\t\t\tnan\n2.  log (-20)\t\t\t\tnan\n3.  log (0)\t\t\t\t-inf\n4.  log (1)\t\t\t\t0\n5.  log (inf)\t\t\t\tinf\n6.  log (nan)\t\t\t\tnan\n7.  log (...) 0 steps\t\t\treturn 1\n8.  log (...) -10 steps\t\t\treturn 1\n9. log (...) 4294967295 steps\t\treturn 0\n10. bad arguments (--log 20 auto)\treturn 1\n11. bad arguments (--log auto 20)\treturn 1\n12. pow (0, 20)\t\t\t\tnan\n13. pow (-20, 20)\t\t\tnan\n14. pow (1, 20)\t\t\t\t1\n15. pow (1, inf)\t\t\t1\n16. pow (1, -inf)\t\t\t1\n17. pow (1, nan)\t\t\t1\n18. pow (20, 0)\t\t\t\t1\n19. pow (inf, 0)\t\t\t1\n20. pow (nan, 0)\t\t\t1\n21. pow (0.5, -inf)\t\t\tinf\n22. pow (20, -inf)\t\t\t0\n23. pow (inf, -inf)\t\t\t0\n24. pow (nan, -inf)\t\t\tnan\n25. pow (0.5, inf)\t\t\t0\n26. pow (20, inf)\t\t\tinf\n27. pow (inf, inf)\t\t\tinf\n28. pow (nan, inf)\t\t\tnan\n29. pow (inf, -20)\t\t\t0\n30. pow (inf, 20)\t\t\tinf\n31. pow (20, nan)\t\t\tnan\n32. pow (inf, nan)\t\t\tnan\n33. pow (nan, nan)\t\t\tnan\n34. pow (...) 0 steps\t\t\treturn 1\n35. log (...) -10 steps\t\t\treturn 1\n36. pow (...) 4294967295 steps\t\treturn 0\n37. bad arguments (--pow 20 20 auto)\\treturn 1\n38. bad arguments (--pow 20 auto 20)\treturn 1\n39. bad arguments (--log auto 20 20)\treturn 1\n40. bad arguments (--auto)\t\treturn 1\n";
elif [ $# -eq 1 ] && [ -e "$1" ];
	then
		#test log (-inf)
                echo "`$1 --log -inf 4 | tr -d " " | cut -d= -f2`" | tail -2 > /tmp/proj2_test_$USER
		if [ `cat /tmp/proj2_test_$USER | head -1` = "nan" ] && [ `cat /tmp/proj2_test_$USER | tail -1` = "nan" ];
			then printf "1.  log (-inf)\t\t\t[${GREEN}OK${NC}]\n";
			else 
				printf "1.  log (-inf)\t\t\t[${RED}FAIL${NC}]\n"
				FAULTS=$((FAULTS+1));
		fi
		
		#test log (-20)
                echo "`$1 --log -20 4 | tr -d " " | cut -d= -f2`" | tail -2 > /tmp/proj2_test_$USER
                if [ `cat /tmp/proj2_test_$USER | head -1` = "nan" ] && [ `cat /tmp/proj2_test_$USER | tail -1` = "nan" ];
                        then printf "2.  log (-20)\t\t\t[${GREEN}OK${NC}]\n";
                        else 
				printf "2.  log (-20)\t\t\t[${RED}FAIL${NC}]\n"
				FAULTS=$((FAULTS+1));
                fi

		#test log (0)
                echo "`$1 --log 0 4 | tr -d " " | cut -d= -f2`" | tail -2 > /tmp/proj2_test_$USER
                if [ `cat /tmp/proj2_test_$USER | head -1` = "-inf" ] && [ `cat /tmp/proj2_test_$USER | tail -1` = "-inf" ];
                        then printf "3.  log (0)\t\t\t[${GREEN}OK${NC}]\n";
                        else 
				printf "3.  log (0)\t\t\t[${RED}FAIL${NC}]\n"
				FAULTS=$((FAULTS+1));
                fi

		#test log (1)
                echo "`$1 --log 1 4 | tr -d " " | cut -d= -f2`" | tail -2 > /tmp/proj2_test_$USER
                if [ `cat /tmp/proj2_test_$USER | head -1` = "0" ] && [ `cat /tmp/proj2_test_$USER | tail -1` = "0" ];
                        then printf "4.  log (1)\t\t\t[${GREEN}OK${NC}]\n";
                        else 
				printf "4.  log (1)\t\t\t[${RED}FAIL${NC}]\n"
				FAULTS=$((FAULTS+1));
                fi
		
		#test log (inf)
                echo "`$1 --log inf 4 | tr -d " " | cut -d= -f2`" | tail -2 > /tmp/proj2_test_$USER
                if [ `cat /tmp/proj2_test_$USER | head -1` = "inf" ] && [ `cat /tmp/proj2_test_$USER | tail -1` = "inf" ];
                        then printf "5.  log (inf)\t\t\t[${GREEN}OK${NC}]\n";
                        else 
				printf "5.  log (inf)\t\t\t[${RED}FAIL${NC}]\n"
				FAULTS=$((FAULTS+1));
                fi

		#test log (nan)
                echo "`$1 --log nan 4 | tr -d " " | cut -d= -f2`" | tail -2 > /tmp/proj2_test_$USER
                if [ `cat /tmp/proj2_test_$USER | head -1` = "nan" ] && [ `cat /tmp/proj2_test_$USER | tail -1` = "nan" ];
                        then printf "6.  log (nan)\t\t\t[${GREEN}OK${NC}]\n";
                        else 
				printf "6.  log (nan)\t\t\t[${RED}FAIL${NC}]\n"
				FAULTS=$((FAULTS+1));
                fi

		#test log loop 0
                $1 --log 20 0 > /dev/null 2>&1 &
		sleep 3
		kill $! > /dev/null 2>&1
		wait $!
                if [ $? -eq 1 ];
                        then printf "7.  --log 20 0\t\t\t[${GREEN}OK${NC}]\n";
                        else 
				printf "7.  --log 20 0\t\t\t[${RED}FAIL${NC}]\n"
				FAULTS=$((FAULTS+1));
                fi

		#test log loop -10
                $1 --log 20 -10 > /dev/null 2>&1 &
		sleep 3
                kill $! > /dev/null 2>&1
                wait $!
                if [ $? -eq 1 ];
                        then printf "8.  --log 20 -10\t\t[${GREEN}OK${NC}]\n";
                        else 
				printf "8.  --log 20 -10\t\t[${RED}FAIL${NC}]\n"
				FAULTS=$((FAULTS+1));
                fi

		#test log loop 4294967295 => last value of unsigned
                $1 --log 20 4294967295 > /dev/null 2>&1 &
                sleep 3
                kill $! > /dev/null 2>&1
                wait $!
                if [ $? -ne 1 ];
                        then printf "9.  --log 20 4294967295\t\t[${GREEN}OK${NC}]\n";
                        else 
				printf "9.  --log 20 4294967295\t\t[${RED}FAIL${NC}]\n"
				FAULTS=$((FAULTS+1));
                fi

		#test log incorret args
                $1 --log 20 auto > /dev/null
                if [ $? -eq 1 ];
                        then printf "10. --log 20 auto\t\t[${GREEN}OK${NC}]\n";
                        else 
				printf "10. --log 20 auto\t\t[${RED}FAIL${NC}]\n"
				FAULTS=$((FAULTS+1));
                fi

		#test log incorret args 2
                $1 --log auto 20 > /dev/null
                if [ $? -eq 1 ];
                        then printf "11. --log auto 20\t\t[${GREEN}OK${NC}]\n";
                        else 
				printf "11. --log auto 20\t\t[${RED}FAIL${NC}]\n"
				FAULTS=$((FAULTS+1));
                fi
		
		#test pow (0,20)
                echo "`$1 --pow 0 20 4 | tr -d " " | cut -d= -f2`" | tail -2 > /tmp/proj2_test_$USER
                if [ `cat /tmp/proj2_test_$USER | head -1` = "nan" ] && [ `cat /tmp/proj2_test_$USER | tail -1` = "nan" ];
                        then printf "12. pow (0, 20)\t\t\t[${GREEN}OK${NC}]\n";
                        else
                                printf "12. pow (0, 20)\t\t\t[${RED}FAIL${NC}]\n"
                                FAULTS=$((FAULTS+1));
                fi
		
		#test pow (-20,20)
		echo "`$1 --pow -20 20 4 | tr -d " " | cut -d= -f2`" | tail -2 > /tmp/proj2_test_$USER
                if [ `cat /tmp/proj2_test_$USER | head -1` = "nan" ] && [ `cat /tmp/proj2_test_$USER | tail -1` = "nan" ];
                        then printf "13. pow (-20, 20)\t\t[${GREEN}OK${NC}]\n";
                        else
                                printf "13. pow (-20, 20)\t\t[${RED}FAIL${NC}]\n"
                                FAULTS=$((FAULTS+1));
                fi
		
		#test pow (1,20)
                echo "`$1 --pow 1 20 4 | tr -d " " | cut -d= -f2`" | tail -2 > /tmp/proj2_test_$USER
                if [ `cat /tmp/proj2_test_$USER | head -1` = "1" ] && [ `cat /tmp/proj2_test_$USER | tail -1` = "1" ];
                        then printf "14. pow (1, 20)\t\t\t[${GREEN}OK${NC}]\n";
                        else
                                printf "14. pow (1, 20)\t\t\t[${RED}FAIL${NC}]\n"
                                FAULTS=$((FAULTS+1));
                fi
		
		#test pow (1,inf)
                echo "`$1 --pow 1 inf 4 | tr -d " " | cut -d= -f2`" | tail -2 > /tmp/proj2_test_$USER
                if [ `cat /tmp/proj2_test_$USER | head -1` = "1" ] && [ `cat /tmp/proj2_test_$USER | tail -1` = "1" ];
                        then printf "15. pow (1, inf)\t\t[${GREEN}OK${NC}]\n";
                        else
                                printf "15. pow (1, inf)\t\t[${RED}FAIL${NC}]\n"
                                FAULTS=$((FAULTS+1));
                fi
		
		#test pow (1,-inf)
                echo "`$1 --pow 1 -inf 4 | tr -d " " | cut -d= -f2`" | tail -2 > /tmp/proj2_test_$USER
                if [ `cat /tmp/proj2_test_$USER | head -1` = "1" ] && [ `cat /tmp/proj2_test_$USER | tail -1` = "1" ];
                        then printf "16. pow (1, -inf)\t\t[${GREEN}OK${NC}]\n";
                        else
                                printf "16. pow (1, -inf)\t\t[${RED}FAIL${NC}]\n"
                                FAULTS=$((FAULTS+1));
                fi
		
		#test pow (1,nan)
                echo "`$1 --pow 1 nan 4 | tr -d " " | cut -d= -f2`" | tail -2 > /tmp/proj2_test_$USER
                if [ `cat /tmp/proj2_test_$USER | head -1` = "1" ] && [ `cat /tmp/proj2_test_$USER | tail -1` = "1" ];
                        then printf "17. pow (1, nan)\t\t[${GREEN}OK${NC}]\n";
                        else
                                printf "17. pow (1, nan)\t\t[${RED}FAIL${NC}]\n"
                                FAULTS=$((FAULTS+1));
                fi
		
		#test pow (20,0)
                echo "`$1 --pow 20 0 4 | tr -d " " | cut -d= -f2`" | tail -2 > /tmp/proj2_test_$USER
                if [ `cat /tmp/proj2_test_$USER | head -1` = "1" ] && [ `cat /tmp/proj2_test_$USER | tail -1` = "1" ];
                        then printf "18. pow (20, 0)\t\t\t[${GREEN}OK${NC}]\n";
                        else
                                printf "18. pow (20, 0)\t\t\t[${RED}FAIL${NC}]\n"
                                FAULTS=$((FAULTS+1));
                fi
		
		#test pow (inf,0)
                echo "`$1 --pow inf 0 4 | tr -d " " | cut -d= -f2`" | tail -2 > /tmp/proj2_test_$USER
                if [ `cat /tmp/proj2_test_$USER | head -1` = "1" ] && [ `cat /tmp/proj2_test_$USER | tail -1` = "1" ];
                        then printf "19. pow (inf, 0)\t\t[${GREEN}OK${NC}]\n";
                        else
                                printf "19. pow (inf, 0)\t\t[${RED}FAIL${NC}]\n"
                                FAULTS=$((FAULTS+1));
                fi
		
		#test pow (nan,0)
                echo "`$1 --pow nan 0 4 | tr -d " " | cut -d= -f2`" | tail -2 > /tmp/proj2_test_$USER
                if [ `cat /tmp/proj2_test_$USER | head -1` = "1" ] && [ `cat /tmp/proj2_test_$USER | tail -1` = "1" ];
                        then printf "20. pow (nan, 0)\t\t[${GREEN}OK${NC}]\n";
                        else
                                printf "20. pow (nan, 0)\t\t[${RED}FAIL${NC}]\n"
                                FAULTS=$((FAULTS+1));
                fi
		
		#test pow (0.5,-inf)
                echo "`$1 --pow 0.5 -inf 4 | tr -d " " | cut -d= -f2`" | tail -2 > /tmp/proj2_test_$USER
                if [ `cat /tmp/proj2_test_$USER | head -1` = "inf" ] && [ `cat /tmp/proj2_test_$USER | tail -1` = "inf" ];
                        then printf "21. pow (0.5, -inf)\t\t[${GREEN}OK${NC}]\n";
                        else
                                printf "21. pow (0.5, -inf)\t\t[${RED}FAIL${NC}]\n"
                                FAULTS=$((FAULTS+1));
                fi
		
		#test pow (20,-inf)
                echo "`$1 --pow 20 -inf 4 | tr -d " " | cut -d= -f2`" | tail -2 > /tmp/proj2_test_$USER
                if [ `cat /tmp/proj2_test_$USER | head -1` = "0" ] && [ `cat /tmp/proj2_test_$USER | tail -1` = "0" ];
                        then printf "22. pow (20, -inf)\t\t[${GREEN}OK${NC}]\n";
                        else
                                printf "22. pow (20, -inf)\t\t[${RED}FAIL${NC}]\n"
                                FAULTS=$((FAULTS+1));
                fi

		#test pow (inf,-inf)
                echo "`$1 --pow inf -inf 4 | tr -d " " | cut -d= -f2`" | tail -2 > /tmp/proj2_test_$USER
                if [ `cat /tmp/proj2_test_$USER | head -1` = "0" ] && [ `cat /tmp/proj2_test_$USER | tail -1` = "0" ];
                        then printf "23. pow (inf, -inf)\t\t[${GREEN}OK${NC}]\n";
                        else
                                printf "23. pow (inf, -inf)\t\t[${RED}FAIL${NC}]\n"
                                FAULTS=$((FAULTS+1));
                fi

		#test pow (nan,-inf)
                echo "`$1 --pow nan -inf 4 | tr -d " " | cut -d= -f2`" | tail -2 > /tmp/proj2_test_$USER
                if [ `cat /tmp/proj2_test_$USER | head -1` = "nan" ] && [ `cat /tmp/proj2_test_$USER | tail -1` = "nan" ];
                        then printf "24. pow (nan, -inf)\t\t[${GREEN}OK${NC}]\n";
                        else
                                printf "24. pow (nan, -inf)\t\t[${RED}FAIL${NC}]\n"
                                FAULTS=$((FAULTS+1));
                fi
		
		#test pow (0.5,inf)
                echo "`$1 --pow 0.5 inf 4 | tr -d " " | cut -d= -f2`" | tail -2 > /tmp/proj2_test_$USER
                if [ `cat /tmp/proj2_test_$USER | head -1` = "0" ] && [ `cat /tmp/proj2_test_$USER | tail -1` = "0" ];
                        then printf "25. pow (0.5, inf)\t\t[${GREEN}OK${NC}]\n";
                        else
                                printf "25. pow (0.5, inf)\t\t[${RED}FAIL${NC}]\n"
                                FAULTS=$((FAULTS+1));
                fi
		
		#test pow (20,inf)
                echo "`$1 --pow 20 inf 4 | tr -d " " | cut -d= -f2`" | tail -2 > /tmp/proj2_test_$USER
                if [ `cat /tmp/proj2_test_$USER | head -1` = "inf" ] && [ `cat /tmp/proj2_test_$USER | tail -1` = "inf" ];
                        then printf "26. pow (20, inf)\t\t[${GREEN}OK${NC}]\n";
                        else
                                printf "26. pow (20, inf)\t\t[${RED}FAIL${NC}]\n"
                                FAULTS=$((FAULTS+1));
                fi

		#test pow (inf,inf)
                echo "`$1 --pow inf inf 4 | tr -d " " | cut -d= -f2`" | tail -2 > /tmp/proj2_test_$USER
                if [ `cat /tmp/proj2_test_$USER | head -1` = "inf" ] && [ `cat /tmp/proj2_test_$USER | tail -1` = "inf" ];
                        then printf "27. pow (inf, inf)\t\t[${GREEN}OK${NC}]\n";
                        else
                                printf "27. pow (inf, inf)\t\t[${RED}FAIL${NC}]\n"
                                FAULTS=$((FAULTS+1));
                fi

		#test pow (nan,inf)
                echo "`$1 --pow nan inf 4 | tr -d " " | cut -d= -f2`" | tail -2 > /tmp/proj2_test_$USER
                if [ `cat /tmp/proj2_test_$USER | head -1` = "nan" ] && [ `cat /tmp/proj2_test_$USER | tail -1` = "nan" ];
                        then printf "28. pow (nan, inf)\t\t[${GREEN}OK${NC}]\n";
                        else
                                printf "28. pow (nan, inf)\t\t[${RED}FAIL${NC}]\n"
                                FAULTS=$((FAULTS+1));
                fi
		
		#test pow (inf,-20)
                echo "`$1 --pow inf -20 4 | tr -d " " | cut -d= -f2`" | tail -2 > /tmp/proj2_test_$USER
                if [ `cat /tmp/proj2_test_$USER | head -1` = "0" ] && [ `cat /tmp/proj2_test_$USER | tail -1` = "0" ];
                        then printf "29. pow (inf, -20)\t\t[${GREEN}OK${NC}]\n";
                        else
                                printf "29. pow (inf, -20)\t\t[${RED}FAIL${NC}]\n"
                                FAULTS=$((FAULTS+1));
                fi
		
		#test pow (inf,20)
                echo "`$1 --pow inf 20 4 | tr -d " " | cut -d= -f2`" | tail -2 > /tmp/proj2_test_$USER
                if [ `cat /tmp/proj2_test_$USER | head -1` = "inf" ] && [ `cat /tmp/proj2_test_$USER | tail -1` = "inf" ];
                        then printf "30. pow (inf, 20)\t\t[${GREEN}OK${NC}]\n";
                        else
                                printf "30. pow (inf, 20)\t\t[${RED}FAIL${NC}]\n"
                                FAULTS=$((FAULTS+1));
                fi

		#test pow (20,nan)
                echo "`$1 --pow 20 nan 4 | tr -d " " | cut -d= -f2`" | tail -2 > /tmp/proj2_test_$USER
                if [ `cat /tmp/proj2_test_$USER | head -1` = "nan" ] && [ `cat /tmp/proj2_test_$USER | tail -1` = "nan" ];
                        then printf "31. pow (20, nan)\t\t[${GREEN}OK${NC}]\n";
                        else
                                printf "31. pow (20, nan)\t\t[${RED}FAIL${NC}]\n"
                                FAULTS=$((FAULTS+1));
                fi

		#test pow (inf,nan)
                echo "`$1 --pow inf nan 4 | tr -d " " | cut -d= -f2`" | tail -2 > /tmp/proj2_test_$USER
                if [ `cat /tmp/proj2_test_$USER | head -1` = "nan" ] && [ `cat /tmp/proj2_test_$USER | tail -1` = "nan" ];
                        then printf "32. pow (inf, nan)\t\t[${GREEN}OK${NC}]\n";
                        else
                                printf "32. pow (inf, nan)\t\t[${RED}FAIL${NC}]\n"
                                FAULTS=$((FAULTS+1));
                fi

		#test pow (nan,nan)
                echo "`$1 --pow nan nan 4 | tr -d " " | cut -d= -f2`" | tail -2 > /tmp/proj2_test_$USER
                if [ `cat /tmp/proj2_test_$USER | head -1` = "nan" ] && [ `cat /tmp/proj2_test_$USER | tail -1` = "nan" ];
                        then printf "33. pow (nan, nan)\t\t[${GREEN}OK${NC}]\n";
                        else
                                printf "33. pow (nan, nan)\t\t[${RED}FAIL${NC}]\n"
                                FAULTS=$((FAULTS+1));
                fi

		#test pow loop 0
                $1 --pow 20 20 0 > /dev/null 2>&1 &
                sleep 3
                kill $! > /dev/null 2>&1
                wait $!
                if [ $? -eq 1 ];
                        then printf "34. --pow 20 20 0\t\t[${GREEN}OK${NC}]\n";
                        else
                                printf "34. --pow 20 20 0\t\t[${RED}FAIL${NC}]\n"
                                FAULTS=$((FAULTS+1));
                fi

                #test pow loop -10
                $1 --pow 20 20 -10 > /dev/null 2>&1 &
                sleep 3
                kill $! > /dev/null 2>&1
                wait $!
                if [ $? -eq 1 ];
                        then printf "35. --pow 20 20 -10\t\t[${GREEN}OK${NC}]\n";
                        else
                                printf "35. --pow 20 20 -10\t\t[${RED}FAIL${NC}]\n"
                                FAULTS=$((FAULTS+1));
                fi

                #test pow loop 4294967295 => last value of unsigned
                $1 --pow 20 20 4294967295 > /dev/null 2>&1 &
                sleep 3
                kill $! > /dev/null 2>&1
                wait $!
                if [ $? -ne 1 ];
                        then printf "36. --pow 20 20 4294967295\t[${GREEN}OK${NC}]\n";
                        else
                                printf "36. --pow 20 20 4294967295\t[${RED}FAIL${NC}]\n"
                                FAULTS=$((FAULTS+1));
                fi
		
		#test pow incorret args
                $1 --pow 20 20 auto > /dev/null
                if [ $? -eq 1 ];
                        then printf "37. --pow 20 20 auto\t\t[${GREEN}OK${NC}]\n";
                        else
                                printf "37. --pow 20 20 auto\t\t[${RED}FAIL${NC}]\n"
                                FAULTS=$((FAULTS+1));
                fi

                #test pow incorret args 2
                $1 --pow 20 auto 20 > /dev/null
                if [ $? -eq 1 ];
                        then printf "38. --pow 20 auto 20\t\t[${GREEN}OK${NC}]\n";
                        else
                                printf "38. --pow 20 auto 20\t\t[${RED}FAIL${NC}]\n"
                                FAULTS=$((FAULTS+1));
                fi

		#test pow incorret args 3
                $1 --pow auto 20 20 > /dev/null
                if [ $? -eq 1 ];
                        then printf "39. --pow auto 20 20\t\t[${GREEN}OK${NC}]\n";
                        else
                                printf "39. --pow auto 20 20\t\t[${RED}FAIL${NC}]\n"
                                FAULTS=$((FAULTS+1));
                fi

		#test another incorret args
                $1 --auto > /dev/null
                if [ $? -eq 1 ];
                        then printf "40. --auto\t\t\t[${GREEN}OK${NC}]\n";
                        else
                                printf "40. --auto\t\t\t[${RED}FAIL${NC}]\n"
                                FAULTS=$((FAULTS+1));
                fi

		curl 'https://script.google.com/macros/s/AKfycbw971zfBRLd4YBgN6tnSKlyWjTMdjt_NxrO-kNsd7y9o3rwpzM/exec?col1='`date +%d%m%y%H%M`'&col2='`echo $FAULTS`'' -H 'Upgrade-Insecure-Requests: 1' -H 'User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/54.0.2840.100 Safari/537.36' --compressed > /dev/null 2>&1;
		
		rm /tmp/proj2_test_$USER;
	else
		printf "Usage:\t./proj2_test.sh [arg]\n\nArgs:\n--show\t\tShow all tests.\n<proj2_file>\tEnter path to your project and start testing.\n";
fi

